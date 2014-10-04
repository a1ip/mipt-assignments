#ifndef LINUX_UTILITIES_THREAD_HPP_
#define LINUX_UTILITIES_THREAD_HPP_

#include <pthread.h>

class Thread {
 public:
  Thread() : state_(STATE_CREATED) { }
  ~Thread() {
    assert(state_ == STATE_CREATED || state_ == STATE_JOINED);
  }

  bool Start(void (*worker)(void*), void* arg = NULL) {
    assert(state_ == STATE_CREATED);
    worker_ = worker;
    arg_ = arg;
    if (pthread_create(&thread_, NULL, ThreadBody, this) != 0) {
      return false;
    }
    state_ = STATE_STARTED;
    return true;
  }

  void Join() {
    assert(state_ == STATE_STARTED);
    int rv = pthread_join(thread_, NULL);
    assert(rv == 0);
    state_ = STATE_JOINED;
  }

 private:
  static void* ThreadBody(void* arg) {
    Thread* thread = reinterpret_cast<Thread*>(arg);
    (thread->worker_)(thread->arg_);
    return NULL;
  }

  enum {
    STATE_CREATED,
    STATE_STARTED,
    STATE_JOINED
  } state_;

  pthread_t thread_;

  void (*worker_)(void*);
  void* arg_;
};

#endif // LINUX_UTILITIES_THREAD_HPP_
