#ifndef _IPC_FILE_BASED_HPP
#define _IPC_FILE_BASED_HPP

#include <stdio.h>

#include <string>

#include "ipc_interface.hpp"
#include "semaphore.hpp"

class FileBasedIpc : public IpcInterface {
 public:
  static FileBasedIpc* Create();
  virtual ~FileBasedIpc();

  virtual bool Close();

  virtual bool InitializeProducer();
  virtual bool InitializeConsumer();

  virtual bool SendMessage(const std::string& message);
  virtual bool ReceiveMessage(std::string* message);

 private:
  FileBasedIpc();

  FILE* files_[2];
  FILE* read_file_;
  FILE* write_file_;

  size_t read_till_;

  Semaphore* sems[4];
  Semaphore* mutex_;
  Semaphore* empty_self_;
  Semaphore* empty_other_;

  enum {
    PARENT,
    PRODUCER,
    CONSUMER
  } status_;
};

#endif /*_IPC_FILE_BASED_HPP*/
