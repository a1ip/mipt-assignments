#ifndef _SYNC_MUTEX_HPP_
#define _SYNC_MUTEX_HPP_

#include <pthread.h>

#include "common.hpp"

class CondVar;

// TODO: add checks verifying that Unlock always happens on the same thread as
// the previous Lock. The same debug variables should be used in AssertHeld.
class Mutex {
 public:
  friend class CondVar;

  Mutex();
  ~Mutex();

  void Lock();
  bool Try();
  void Unlock();

  void AssertHeld() const;

 private:
  pthread_mutex_t mutex_;

  DISALLOW_COPY_AND_ASSIGN(Mutex);
};

class ScopedMutex {
 public:
  explicit ScopedMutex(Mutex *m);
  ~ScopedMutex();

 private:
  Mutex* mutex_;

  DISALLOW_COPY_AND_ASSIGN(ScopedMutex);
};

#endif /*_SYNC_MUTEX_HPP_*/

