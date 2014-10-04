#ifndef _SYNC_SYNCHRONIZED_QUEUE_HPP_
#define _SYNC_SYNCHRONIZED_QUEUE_HPP_

#include <queue>

#include "common.hpp"
#include "condition_variable.hpp"
#include "mutex.hpp"

class SynchronizedQueue {
 public:
  SynchronizedQueue();
  ~SynchronizedQueue();

  void Put(void* message);
  void* Get();

  size_t Size();

 private:
  DISALLOW_COPY_AND_ASSIGN(SynchronizedQueue);

  std::queue<void*> queue_;
  Mutex mutex_;
  CondVar condvar_;
};

#endif /*_SYNC_SYNCHRONIZED_QUEUE_HPP_*/

