#include <stdio.h>

#include <memory>
#include <string>
#include <vector>

#include "error.hpp"

#include "ipc_pipe_based.hpp"

PipeBasedIpc::PipeBasedIpc() {
  for(size_t i = 0; i < 4; i++) {
    fds_[i] = -1;
  }
}

PipeBasedIpc::~PipeBasedIpc() {
  Close();
}

PipeBasedIpc* PipeBasedIpc::Create() {
  std::auto_ptr<PipeBasedIpc> result(new PipeBasedIpc());
  if(result.get() == NULL) {
    error::ReportError(error::ERROR_MEMORY);
    return NULL;
  }

  if(pipe(result->fds_) == -1) {
    return NULL;
  }
  if(pipe(&(result->fds_[2])) == -1) {
    return NULL;
  }

  return result.release();
}

bool PipeBasedIpc::Close() {
  bool result = true;

  for(size_t i = 0; i < 4; i++) {
    if(fds_[i] != -1) {          
      if(close(fds_[i]) == -1) {
        result &= false;
      }
      fds_[i] = -1;
    }
  }

  return result;
}

bool PipeBasedIpc::InitializeProducer() {
  if(close(fds_[2]) == -1) {
    return false;
  }
  fds_[2] = -1;
  if(close(fds_[1]) == -1) {
    return false;
  }
  fds_[1] = -1;

  read_fd_ = fds_[0];
  write_fd_ = fds_[3];

  return true;
}

bool PipeBasedIpc::InitializeConsumer() {
  if(close(fds_[0]) == -1) {
    return false;
  }
  fds_[0] = -1;
  if(close(fds_[3]) == -1) {
    return false;
  }
  fds_[3] = -1;

  read_fd_ = fds_[2];
  write_fd_ = fds_[1];

  return true;
}

bool PipeBasedIpc::SendMessage(const std::string& message) {
  size_t message_size = message.size();
  if(write(write_fd_, reinterpret_cast<char*>(&message_size), sizeof(message_size)) != sizeof(message_size)) {
    return false;
  }
  if(write(write_fd_, message.c_str(), message_size) != message_size) {
    return false;
  }
  return true;
}

bool PipeBasedIpc::ReceiveMessage(std::string* message) {
  size_t message_size;
  if(read(read_fd_, reinterpret_cast<char*>(&message_size), sizeof(message_size)) != sizeof(message_size)) {
    return false;
  }

  char static_buffer[128];
  std::vector<char> dynamic_buffer;
  char* buffer;

  if(message_size <= 128) {
    buffer = static_buffer;
  } else {
    dynamic_buffer.resize(message_size);
    buffer = &dynamic_buffer[0];
  }

  if(read(read_fd_, buffer, message_size) != message_size) {
    return false;
  }

  message->assign(buffer, message_size);

  return true;
}

