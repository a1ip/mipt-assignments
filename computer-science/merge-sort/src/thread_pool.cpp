#include "thread_pool.hpp"

#include <assert.h>

#include <vector>

#include "condition_variable.hpp"
#include "mutex.hpp"
#include "synchronized_queue.hpp"
#include "thread.hpp"

void Task::Start() {
  Run();
}

WaitableTask::WaitableTask() : done_(false) { }

void WaitableTask::Start() {
  Run();
  ScopedMutex m(&mutex_);
  done_ = true;
  cond_.Signal();
}

void WaitableTask::Wait() {
  ScopedMutex m(&mutex_);
  while(!done_) {
    cond_.Wait(&mutex_);
  }
}

ThreadPool::ThreadPool(size_t num_workers) : state_(THREADPOOL_INITIALIZED), num_workers_(num_workers) { }

ThreadPool::~ThreadPool() {
  assert(state_ == THREADPOOL_SHUT_DOWN);
}

bool ThreadPool::Start() {
  assert(state_ == THREADPOOL_INITIALIZED);

  for(size_t i = 0; i < num_workers_; i++) {
    Thread* worker = new Thread;
    assert(worker != NULL);
    if(!worker->Start(&WorkerBody, this)) {
      return false;
    }
    workers_.push_back(worker);
  }

  state_ = THREADPOOL_STARTED;

  return true;
}

void ThreadPool::PostTask(Task* task) {
  assert(state_ == THREADPOOL_STARTED);
  assert(task != NULL);
  queue_.Put(task);
}

void ThreadPool::ShutDown() {
  assert(state_ == THREADPOOL_STARTED);
  state_ = THREADPOOL_SHUTTING_DOWN;

  size_t size = workers_.size();
  for(size_t i = 0; i < size; i++) {
    queue_.Put(NULL);
  }
  for(size_t i = 0; i < size; i++) {
    workers_[i]->Join();
    delete workers_[i];
  }

  state_ = THREADPOOL_SHUT_DOWN;
}

void ThreadPool::WorkerBody(void* arg) {
  assert(arg != NULL);
  ThreadPool* pool = static_cast<ThreadPool*>(arg);
  
  while(true) {
    Task* task = static_cast<Task*>(pool->queue_.Get());
    if(task == NULL) {
      return;
    }
    task->Start();
  }
}

