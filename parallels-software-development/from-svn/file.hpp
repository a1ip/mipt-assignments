#ifndef FILE_HPP_
#define FILE_HPP_

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "logger.hpp"

class File {
 public:
  enum Mode {
    MODE_READ,
    MODE_WRITE
  };

  File() : state_(STATE_CLOSED) { }
  ~File() {
    if (state_ == STATE_OPENED) {
      Close();
    }
  }

  bool Open(const char* file, Mode mode) {
    assert(state_ == STATE_CLOSED);

    mode_ = mode;

    if (mode == MODE_READ) {
      fd_ = open(file, O_RDONLY);
    } else {
      mode_t file_mode = 0644;
      fd_ = open(file, O_RDWR | O_CREAT | O_TRUNC, file_mode);
    }

    if (fd_ == -1) {
      Logger::GetInstance().Log(Logger::LEVEL_ERROR, "Couldn't open file '%s'.", file);
      return false;
    }

    struct stat status;
    int rv = fstat(fd_, &status);
    if (rv != 0) {
      Logger::GetInstance().Log(Logger::LEVEL_ERROR, "Couldn't read stats of file '%d'.", fd_);
      return false;
    }
    size_ = static_cast<size_t>(status.st_size);

    state_ = STATE_OPENED;
    return true;
  }

  bool Close() {
    assert(state_ == STATE_OPENED);

    int rv = close(fd_);
    if (rv != 0) {
      Logger::GetInstance().Log(Logger::LEVEL_WARNING, "Couldn't close file descriptor '%d'.", fd_);
      return false;
    }
    
    state_ = STATE_CLOSED;
    return true;
  }

  int GetFD() const {
    return fd_;
  }

  size_t GetSize() const {
    assert(state_ == STATE_OPENED);
    return size_;
  }

  bool SetSize(size_t size) {
    assert(state_ == STATE_OPENED);
    assert(mode_ == MODE_WRITE);

    if (size_ == size) {
      return true;
    }

    int rv = ftruncate(fd_, size);
    if (rv != 0) {
      Logger::GetInstance().Log(Logger::LEVEL_ERROR, "Couldn't truncate file '%d'.", fd_);
      return false;
    }

    size_ = size;
    return true;
  }

 private:
  int fd_;

  size_t size_;

  Mode mode_;
  enum {
    STATE_CLOSED,
    STATE_OPENED
  } state_;
};

#endif // FILE_HPP_
