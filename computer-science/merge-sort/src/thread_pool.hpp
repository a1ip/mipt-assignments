#ifndef _SYNC_THREAD_POOL_HPP_
#define _SYNC_THREAD_POOL_HPP_

#include <vector>

#include "common.hpp"
#include "condition_variable.hpp"
#include "mutex.hpp"
#include "synchronized_queue.hpp"
#include "thread.hpp"

class Task {
 public:
  Task() { };
  virtual ~Task() { };

  // Override this in the derived class.
  virtual void Run() = 0;

  // Starts the task's execution.
  // Should not be called.
  virtual void Start();
};

class WaitableTask : public Task {
 public:
  WaitableTask();
  virtual ~WaitableTask() { };

  // Starts the task's execution.
  // Should not be called.
  virtual void Start();

  // Override this in the derived class.
  virtual void Run() = 0;

  // Waits until the task is done.
  void Wait();

 private:
  Mutex mutex_;
  CondVar cond_;
  bool done_;
};

class ThreadPool {
 public:
  explicit ThreadPool(size_t num_workers);
  ~ThreadPool();

  // Starts ThreadPool.
  // Returns 'true' on success, 'false' on error.
  bool Start();

  // Waits until all the queued tasks are done,
  // then destroys ThreadPool.
  void ShutDown();

  // Add a task to the execution queue.
  void PostTask(Task* task);

 private:
  DISALLOW_COPY_AND_ASSIGN(ThreadPool);

  static void WorkerBody(void* arg);

  size_t num_workers_;
  std::vector<Thread*> workers_;
  SynchronizedQueue queue_;

  enum {
    THREADPOOL_INITIALIZED,
    THREADPOOL_STARTED,
    THREADPOOL_SHUTTING_DOWN,
    THREADPOOL_SHUT_DOWN
  } state_;

  Mutex state_mutex_;
};

#endif /*_SYNC_THREAD_POOL_HPP_*/

