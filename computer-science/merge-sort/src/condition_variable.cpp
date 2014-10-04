#include "condition_variable.hpp"

#include <assert.h>
#include <pthread.h>

#include "mutex.hpp"

CondVar::CondVar() {
  int rv = pthread_cond_init(&cond_, NULL);
  assert(rv == 0);
}

CondVar::~CondVar() {
  int rv = pthread_cond_destroy(&cond_);
  assert(rv == 0);
}

void CondVar::Signal() {
  int rv = pthread_cond_signal(&cond_);
  assert(rv == 0);
}

void CondVar::Broadcast() {
  int rv = pthread_cond_broadcast(&cond_);
  assert(rv == 0);
}

void CondVar::Wait(Mutex* mutex) {
  assert(mutex != NULL);

  int rv = pthread_cond_wait(&cond_, &mutex->mutex_);
  assert(rv == 0);
}

/*
#include <errno.h>
#include <time.h>

bool CondVar::TimedWait(Mutex* mutex, int timeout) {
  assert(mutex != NULL);
  assert(timeout >= 0);
  assert(status_ == CONDVAR_CREATED);

  struct timespec ts;
  int rv = clock_gettime(CLOCK_REALTIME, &ts);
  assert(rv == 0);

  const int ms_in_s = 1000;
  const int ns_in_ms = 1000 * 1000;

  ts.tv_sec += timeout / ms_in_s;
  timeout = timeout % ms_in_s;
  ts.tv_nsec += timeout * ns_in_ms;
  ts.tv_sec += ts.tv_nsec / (ns_in_ms * ms_in_s);
  ts.tv_nsec %= ns_in_ms * ms_in_s;

  rv = pthread_cond_timedwait(&cond_, &mutex->mutex_, &ts);
  assert(rv == 0 || rv == ETIMEDOUT);

  return rv == ETIMEDOUT;
}
*/

