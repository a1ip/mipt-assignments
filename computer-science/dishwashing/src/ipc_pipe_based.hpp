#ifndef _IPC_PIPE_BASED_HPP
#define _IPC_PIPE_BASED_HPP

#include <string>

#include "ipc_interface.hpp"

class PipeBasedIpc : public IpcInterface {
 public:
  static PipeBasedIpc* Create();
  virtual ~PipeBasedIpc();

  virtual bool Close();

  virtual bool InitializeProducer();
  virtual bool InitializeConsumer();

  virtual bool SendMessage(const std::string& message);
  virtual bool ReceiveMessage(std::string* message);

 private:
  PipeBasedIpc();

  int fds_[4];
  int read_fd_;
  int write_fd_;
};

#endif /*_IPC_PIPE_BASED_HPP*/

