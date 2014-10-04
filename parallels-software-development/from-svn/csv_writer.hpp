#ifndef CSV_WRITER_HPP_
#define CSV_WRITER_HPP_

#include <string.h>

#include <string>

#include "file.hpp"
#include "logger.hpp"
#include "mapped_file.hpp"
#include "utility.hpp"

class CSVWriter {
 public:
  CSVWriter() : state_(STATE_CLOSED) { }
  ~CSVWriter() {
    if (state_ == STATE_OPENED) {
      Close();
      state_ = STATE_CLOSED;
    }
  }

  bool Open(const char* file_name) {
    assert(state_ == STATE_CLOSED);

    bool rv = mapped_file_.Open(file_name, MappedFile::MODE_WRITE);
    if (rv == false) {
      return false;
    }
    position_ = 0;

    size_ = 0;
    capacity_ = initial_capacity;

    rv = mapped_file_.SetSize(capacity_);
    if (rv == false) {
      return false;
    }

    state_ = STATE_OPENED;
    return true;
  }

  bool Close() {
    assert(state_ == STATE_OPENED);

    bool rv = mapped_file_.SetSize(size_);
    if (rv == false) {
      return false;
    }

    rv = mapped_file_.Close();
    if (rv == false) {
      return false;
    }

    state_ = STATE_CLOSED;
    return true;
  }

  bool WriteNext(int32_t value) {
    assert(state_ == STATE_OPENED);

    sprintf(buffer, "%d,", value);
    size_t length = strlen(buffer);

    bool rv = EnsureCapacity(size_ + length);
    if (rv == false) {
      return false;
    }

    for (size_t i = 0; i < length; i++) {
      bool rv = mapped_file_.Set(size_ + i, buffer[i]);
      if (rv == false) {
        return false;
      }
    }

    size_ += length;

    return true;
  }

 private:
  bool EnsureCapacity(size_t capacity) {
    if (capacity_ >= capacity) {
      return true;
    }
    while (capacity_ < capacity) {
      capacity_ += capacity_;
    }
    bool rv = mapped_file_.SetSize(capacity_);
    if (rv == false) {
      return false;
    }
    return true;
  }

  MappedFile mapped_file_;
  size_t position_;

  size_t size_;
  size_t capacity_;

  static const size_t initial_capacity = 128;

  static const size_t buffer_size = 32;
  char buffer[buffer_size];

  enum {
    STATE_CLOSED,
    STATE_OPENED
  } state_;
};

#endif // CSV_WRITER_HPP_
