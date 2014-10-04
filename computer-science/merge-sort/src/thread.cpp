#include "thread.hpp"

#include <assert.h>
#include <pthread.h>

Thread::Thread() : state_(THREAD_INITIALIZED) { }

Thread::~Thread() {
  assert(state_ == THREAD_JOINED || THREAD_INITIALIZED);
}

bool Thread::Start(void (*worker)(void*), void* arg) {
  assert(state_ == THREAD_INITIALIZED);

  worker_ = worker;
  arg_ = arg;

  state_ = THREAD_STARTED;

  if(pthread_create(&thread_, NULL, ThreadBody, this) != 0) {
    return false;
  }

  return true;
}

void Thread::Join()  {
  assert(state_ == THREAD_STARTED);

  int rv = pthread_join(thread_, NULL);
  assert(rv == 0);

  state_ = THREAD_JOINED;
}

void* Thread::ThreadBody(void* arg) {
  Thread* thread = reinterpret_cast<Thread*>(arg);
  (thread->worker_)(thread->arg_);
  return NULL;
}

