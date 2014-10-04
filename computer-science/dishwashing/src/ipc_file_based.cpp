#include <stdio.h>

#include <memory>
#include <string>
#include <vector>

#include "error.hpp"
#include "semaphore.hpp"

#include "ipc_file_based.hpp"

FileBasedIpc::FileBasedIpc() {
  status_ = PARENT;

  files_[0] = NULL;
  files_[1] = NULL;
  read_file_ = NULL;
  write_file_ = NULL;
  read_till_ = 0;

  for(size_t i = 0; i < 4; i++) {
    sems[i] = NULL;
  }

  mutex_ = NULL;
  empty_self_ = NULL;
  empty_other_ = NULL;
}

FileBasedIpc::~FileBasedIpc() {
  Close();
}

FileBasedIpc* FileBasedIpc::Create() {
  std::auto_ptr<FileBasedIpc> result(new FileBasedIpc());
  if(result.get() == NULL) {
    error::ReportError(error::ERROR_MEMORY);
    return NULL;
  }

  for(size_t i = 0; i < 4; i++) {
    result->sems[i] = Semaphore::Create();
    if(result->sems[i] == NULL) {
      return NULL;
    }
  }

  if(!result->sems[0]->Signal()) {
    return NULL;
  }
  if(!result->sems[2]->Signal()) {
    return NULL;
  }

  result->files_[0] = tmpfile();
  if(result->files_[0] == NULL) {
    return NULL;
  }
  result->files_[1] = tmpfile();
  if(result->files_[1] == NULL) {
    return NULL;
  }

  return result.release();
}

bool FileBasedIpc::Close() {
  std::auto_ptr<Semaphore> temp0(sems[0]);
  std::auto_ptr<Semaphore> temp1(sems[1]);
  std::auto_ptr<Semaphore> temp2(sems[2]);
  std::auto_ptr<Semaphore> temp3(sems[3]);

  bool result = true;

  for(size_t i = 0; i < 4; i++) {
    if(sems[i] != NULL) {
      if(status_ == PARENT) {
        if(!sems[i]->Destroy()) {
          result &= false;
        }
      }
      sems[i] = NULL;
    }
  }
  for(size_t i = 0; i < 2; i++) {
    if(files_[i] != NULL) {
      if(fclose(files_[i]) != 0) {
        result &= false;
      }
      files_[i] = NULL;
    }
  }

  return result;
}

bool FileBasedIpc::InitializeProducer() {
  status_ = PRODUCER;
  mutex_ = sems[0];
  empty_self_ = sems[1];
  empty_other_ = sems[3];
  write_file_ = files_[0];
  read_file_ = files_[1];
  return true;
}

bool FileBasedIpc::InitializeConsumer() {
  status_ = CONSUMER;
  mutex_ = sems[2];
  empty_self_ = sems[3];
  empty_other_ = sems[1];
  write_file_ = files_[1];
  read_file_ = files_[0];
  return true;
}

bool FileBasedIpc::SendMessage(const std::string& message) {
  if(!mutex_->Wait()) {
    return false;
  }

  if(fseek(write_file_, 0, SEEK_END) != 0) {
    return false;
  }
  
  size_t message_size = message.size();

  if(fwrite(&message_size, 1, sizeof(message_size), write_file_) != sizeof(message_size)) {
    return false;
  }
  if(fwrite(message.c_str(), 1, message_size, write_file_) != message_size) {
    return false;
  }

  if(fflush(write_file_) != 0) {
    return false;
  }

  if(!mutex_->Signal()) {
    return false;
  }
  if(!empty_other_->Signal()) {
    return false;
  }

  return true;
}

bool FileBasedIpc::ReceiveMessage(std::string* message) {
  if(!empty_self_->Wait()) {
    return false;
  }
  if(!mutex_->Wait()) {
    return false;
  }

  if(fseek(read_file_, read_till_, SEEK_SET) != 0) {
    return false;
  }

  size_t message_size;
  if(fread(&message_size, 1, sizeof(message_size), read_file_) != sizeof(message_size)) {
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

  if(fread(buffer, 1, message_size, read_file_) != message_size) {
    return false;
  }

  message->assign(buffer, message_size);

  read_till_ += sizeof(message_size) + message_size;

  if(!mutex_->Signal()) {
    return false;
  }

  return true;
}

