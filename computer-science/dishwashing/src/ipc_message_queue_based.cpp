#include <stdio.h>
#include <string.h>
#include <sys/msg.h>

#include <memory>
#include <string>

#include "error.hpp"

#include "ipc_message_queue_based.hpp"

MessageQueueBasedIpc::MessageQueueBasedIpc() {
  status_ = PARENT;
  queues_[0] = -1;
  queues_[1] = -1;
}

MessageQueueBasedIpc::~MessageQueueBasedIpc() {
  Close();
}

MessageQueueBasedIpc* MessageQueueBasedIpc::Create() {
  std::auto_ptr<MessageQueueBasedIpc> result(new MessageQueueBasedIpc());
  if(result.get() == NULL) {
    error::ReportError(error::ERROR_MEMORY);
    return NULL;
  }

  for(size_t i = 0; i < 2; i++) {
    result->queues_[i] = msgget(IPC_PRIVATE, 0600 | IPC_CREAT | IPC_EXCL);
    if(result->queues_[i] == -1) {
      return NULL;
    }
  }

  return result.release();
}

bool MessageQueueBasedIpc::Close() {
  bool result = true;

  for(size_t i = 0; i < 2; i++) {
    if(queues_[i] != -1) {
      if(status_ == PARENT) {
        if(msgctl(queues_[i], IPC_RMID, NULL) != 0) {
          result &= false;
        }
      }
      queues_[i] = -1;
    }
  }

  return result;
}

bool MessageQueueBasedIpc::InitializeProducer() {
  status_ = PRODUCER;
  read_queue_ = queues_[0];
  write_queue_ = queues_[1];
  return true;
}

bool MessageQueueBasedIpc::InitializeConsumer() {
  status_ = CONSUMER;
  read_queue_ = queues_[1];
  write_queue_ = queues_[0];
  return true;
}

bool MessageQueueBasedIpc::SendMessage(const std::string& message) {
  if(message.size() > Message::BUFFER_SIZE) {
    error::ReportError(error::ERROR_MSG_SIZE);
    return false;
  }

  struct Message msg;
  msg.type = 1;
  if(strcpy(msg.data, message.c_str()) == NULL) {
    return false;
  }

  if(msgsnd(write_queue_, &msg, message.size(), 0) != 0) {
    return false;
  }

  return true;
}

bool MessageQueueBasedIpc::ReceiveMessage(std::string* message) {
  struct Message msg;

  int message_size = msgrcv(read_queue_, &msg, Message::BUFFER_SIZE, 0, 0);
  if(message_size < 0) {
    return false;
  }

  message->assign(msg.data, message_size);

  return true;
}

