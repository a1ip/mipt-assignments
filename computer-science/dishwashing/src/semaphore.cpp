#include <stdio.h>
#include <sys/sem.h>

#include <memory>

#include "error.hpp"

#include "semaphore.hpp"

Semaphore* Semaphore::Create() {
  std::auto_ptr<Semaphore> result(new Semaphore());
  if(result.get() == NULL) {
    error::ReportError(error::ERROR_MEMORY);
    return NULL;
  }

  result->destroyed_ = false;

  result->sem_ = semget(IPC_PRIVATE, 1, 0600 | IPC_CREAT | IPC_EXCL);
  if(result->sem_ == -1) {
    return false;
  }

  return result.release();
}

Semaphore::Semaphore() { }

Semaphore::~Semaphore() { }

bool Semaphore::Signal() {
  if(destroyed_) {
    error::ReportError(error::ERROR_SEMAPHORE);
    return false;
  }

  struct sembuf mybuf;
  mybuf.sem_op = 1;
  mybuf.sem_flg = 0;
  mybuf.sem_num = 0;

  if(semop(sem_, &mybuf, 1) != 0) {
    return false;
  }

  return true;
}

bool Semaphore::Wait() {
  // TODO assert
  if(destroyed_) {
    error::ReportError(error::ERROR_SEMAPHORE);
    return false;
  }

  struct sembuf mybuf;
  mybuf.sem_op = -1;
  mybuf.sem_flg = 0;
  mybuf.sem_num = 0;

  if(semop(sem_, &mybuf, 1) != 0) {
    return false;
  }

  return true;
}

bool Semaphore::Destroy() {
  if(destroyed_ == false) {
    if(semctl(sem_, 0, IPC_RMID, 0) != 0) {
      return false;
    }

    destroyed_ = true;
  }
  return true;
}

