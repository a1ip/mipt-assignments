#include "synchronized_queue.hpp"

#include <assert.h>

#include <queue>

#include "condition_variable.hpp"
#include "mutex.hpp"

SynchronizedQueue::SynchronizedQueue() { }

SynchronizedQueue::~SynchronizedQueue() {
  assert(queue_.empty());
}

void SynchronizedQueue::Put(void* message) {
  ScopedMutex m(&mutex_);
  queue_.push(message);
  condvar_.Signal();
}

void* SynchronizedQueue::Get() {
  ScopedMutex l(&mutex_);
  while(queue_.empty()) {
    condvar_.Wait(&mutex_);
  }
  void* result = queue_.front();
  queue_.pop();
  return result;
}

size_t SynchronizedQueue::Size() {
  ScopedMutex l(&mutex_);
  return queue_.size();
}

