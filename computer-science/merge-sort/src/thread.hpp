#ifndef _SYNC_THREAD_HPP_
#define _SYNC_THREAD_HPP_

#include "common.hpp"

#include <pthread.h>

class Thread {
 public:
  Thread();
  ~Thread();

  bool Start(void (*worker)(void*), void* arg = NULL);
  void Join();

 private:
  DISALLOW_COPY_AND_ASSIGN(Thread);

  static void* ThreadBody(void* arg);

  enum {
    THREAD_INITIALIZED,
    THREAD_STARTED,
    THREAD_JOINED
  } state_;

  pthread_t thread_;

  void (*worker_)(void* arg);
  void* arg_;
};

#endif /*_SYNC_THREAD_HPP_*/
