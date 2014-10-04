#ifndef _SYNC_CONDITION_VARIABLE_HPP_
#define _SYNC_CONDITION_VARIABLE_HPP_

#include <pthread.h>

#include "common.hpp"

class Mutex;

class CondVar {
 public:
  CondVar();
  ~CondVar();

  void Signal();
  void Broadcast();

  void Wait(Mutex* mutex);
  //bool TimedWait(Mutex* mutex, int timeout);

 private:
  DISALLOW_COPY_AND_ASSIGN(CondVar);

  pthread_cond_t cond_;
};

#endif /*_SYNC_CONDITION_VARIABLE_HPP_*/

