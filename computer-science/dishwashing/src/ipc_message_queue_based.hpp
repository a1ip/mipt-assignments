#ifndef _IPC_MESSAGE_QUEUE_BASED_HPP
#define _IPC_MESSAGE_QUEUE_BASED_HPP

#include <string>

#include "ipc_interface.hpp"

class MessageQueueBasedIpc : public IpcInterface {
 public:
  static MessageQueueBasedIpc* Create();
  virtual ~MessageQueueBasedIpc();

  virtual bool Close();

  virtual bool InitializeProducer();
  virtual bool InitializeConsumer();

  virtual bool SendMessage(const std::string& message);
  virtual bool ReceiveMessage(std::string* message);

 private:
  struct Message {
    static const int BUFFER_SIZE = 1024;

    long type;
    char data[BUFFER_SIZE];
  };

  MessageQueueBasedIpc();

  int queues_[2];
  int read_queue_;
  int write_queue_;

  enum {
    PARENT,
    PRODUCER,
    CONSUMER
  } status_;
};

#endif /*_IPC_MESSAGE_QUEUE_BASED_HPP*/

