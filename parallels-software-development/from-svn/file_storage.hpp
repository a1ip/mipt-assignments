#ifndef FILE_STORAGE_HPP_
#define FILE_STORAGE_HPP_

#include <stdint.h>

#include "mapped_file.hpp"

class FileStorage {
 public:
  FileStorage() : state_(STATE_CLOSED) { }
  ~FileStorage() {
    if (state_ == STATE_OPENED) {
      Close();
      state_ = STATE_CLOSED;
    }
  }

  bool Open(const char* file_name, size_t size = 0) {
    assert(state_ == STATE_CLOSED);

    bool rv = mapped_file_.Open(file_name, MappedFile::MODE_WRITE);
    if (rv == false) {
      return false;
    }

    size_ = 0;
    capacity_ = initial_capacity;

    rv = mapped_file_.SetSize(capacity_ * sizeof(int32_t));
    if (rv == false) {
      return false;
    }

    state_ = STATE_OPENED;

    rv = SetSize(size);
    if (rv == false) {
      return false;
    }

    return true;
  }

  bool Close() {
    assert(state_ == STATE_OPENED);

    bool rv = mapped_file_.SetSize(size_ * sizeof(int32_t));
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

  bool Set(size_t index, int32_t value) {
    assert(state_ == STATE_OPENED);
    assert(index < size_);

    for (size_t i = 0; i < sizeof(value); i++) {
      bool rv = mapped_file_.Set(index * sizeof(value) + i,
        reinterpret_cast<char*>(&value)[i]);
      if (rv == false) {
        return false;
      }
    }

    return true;
  }

  bool Get(size_t index, int32_t* value) {
    assert(state_ == STATE_OPENED);
    assert(index < size_);
    assert(value != NULL);

    for (size_t i = 0; i < sizeof(*value); i++) {
      bool rv = mapped_file_.Get(index * sizeof(*value) + i,
        reinterpret_cast<char*>(value) + i);
      if (rv == false) {
        return false;
      }
    }

    return true;
  }

  bool PushBack(int32_t value) {
    assert(state_ == STATE_OPENED);

    bool rv = SetSize(size_ + 1);
    if (rv == false) {
      return false;
    }

    rv = Set(size_ - 1, value);
    if (rv == false) {
      return false;
    }

    return true;
  }

  bool SetSize(size_t size) {
    assert(state_ == STATE_OPENED);

    if (size_ == size) {
      return true;
    }

    bool rv = EnsureCapacity(size);
    if (rv == false) {
      return false;
    }

    size_ = size;
    return true;
  }

  size_t GetSize() const {
    assert(state_ == STATE_OPENED);
    return size_;
  }

 private:
  bool EnsureCapacity(size_t capacity) {
    assert(state_ == STATE_OPENED);

    if (capacity_ >= capacity) {
      return true;
    }

    while (capacity_ < capacity) {
      capacity_ += capacity_;
    }

    bool rv = mapped_file_.SetSize(capacity_ * sizeof(int32_t));
    if (rv == false) {
      return false;
    }

    return true;
  }

  MappedFile mapped_file_;

  size_t size_;
  size_t capacity_;

  static const size_t initial_capacity = 128 * 1024;

  enum {
    STATE_CLOSED,
    STATE_OPENED
  } state_;
};

#endif // FILE_STORAGE_HPP_
