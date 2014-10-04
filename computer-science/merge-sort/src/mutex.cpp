#include "mutex.hpp"

#include <assert.h>
#include <pthread.h>

#include <memory>

Mutex::Mutex()  {
  int rv = pthread_mutex_init(&mutex_, NULL);
  assert(rv == 0);
}

Mutex::~Mutex() {
  int rv = pthread_mutex_destroy(&mutex_);
  assert(rv == 0);
}

void Mutex::Lock() {
  int rv = pthread_mutex_lock(&mutex_);
  assert(rv == 0);
}

bool Mutex::Try() {
  return (pthread_mutex_trylock(&mutex_) == 0);
}

void Mutex::AssertHeld() const {
  // TODO: implement.
}

void Mutex::Unlock() {
  int rv = pthread_mutex_unlock(&mutex_);
  assert(rv == 0);
}

ScopedMutex::ScopedMutex(Mutex *mutex): mutex_(mutex) {
  assert(mutex_ != NULL);
  mutex_->Lock();
}

ScopedMutex::~ScopedMutex() {
  mutex_->Unlock();
}

