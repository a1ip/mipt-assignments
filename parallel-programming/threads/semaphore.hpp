#ifndef LINUX_UTILITIES_SEMAPHORE_HPP
#define LINUX_UTILITIES_SEMAPHORE_HPP

#include <assert.h>

#include <semaphore.h>

class Semaphore {
 public:
  Semaphore() : state_(STATE_FINALIZED) { }
  ~Semaphore() {
    if (state_ == STATE_INITIALIZED) {
      Finalize();
    }
  }

  bool Initialize(unsigned int value) {
    assert(state_ == STATE_FINALIZED);
    int rv = sem_init(&sem_, 0, value);
    if (rv != 0) {
      return false;
    }
    state_ = STATE_INITIALIZED;
    return true;
  }

  bool Signal() {
    assert(state_ == STATE_INITIALIZED);
    int rv = sem_post(&sem_);
    if (rv != 0) {
      return false;
    }
    return true;
  }

  bool Wait() {
    assert(state_ == STATE_INITIALIZED);
    int rv = sem_wait(&sem_);
    if (rv != 0) {
      return false;
    }
    return true;
  }

  bool Finalize() {
    assert(state_ == STATE_INITIALIZED);
    int rv = sem_destroy(&sem_);
    if (rv != 0) {
      return false;
    }
    state_ = STATE_FINALIZED;
    return true;
  }

 private:
  sem_t sem_;

  enum {
    STATE_FINALIZED,
    STATE_INITIALIZED
  } state_;
};

#endif // LINUX_UTILITIES_SEMAPHORE_HPP
