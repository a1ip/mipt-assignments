#ifndef _SEMAPHORE_HPP
#define _SEMAPHORE_HPP

class Semaphore {
 public:
  static Semaphore* Create();
  ~Semaphore();

  bool Signal();
  bool Wait();

  bool Destroy();

 private:
  Semaphore();

  bool destroyed_;
  int sem_;
};

#endif /*_SEMAPHORE_HPP*/

