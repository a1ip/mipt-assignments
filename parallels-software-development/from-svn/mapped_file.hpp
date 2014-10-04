#ifndef MAPPED_FILE_HPP_
#define MAPPED_FILE_HPP_

#include "file.hpp"
#include "file_mapper.hpp"
#include "utility.hpp"

class MappedFile {
 public:
  enum Mode {
    MODE_READ,
    MODE_WRITE
  };

  MappedFile() : state_(STATE_CLOSED) { }
  ~MappedFile() {
    if (state_ == STATE_OPENED || state_ == STATE_MAPPED) {
      Close();
    }
  }

  bool Open(const char* filename, Mode mode) {
    assert(state_ == STATE_CLOSED);
    assert(filename != NULL);

    File::Mode file_mode = (mode == MODE_READ ?
      File::MODE_READ : File::MODE_WRITE);
    bool rv = file_.Open(filename, file_mode);
    if (rv == false) {
      return false;
    }

    FileMapper::Mode mapper_mode = (mode == MODE_READ ?
      FileMapper::MODE_READ : FileMapper::MODE_WRITE);
    rv = mapper_.Open(&file_, mapper_mode);
    if (rv == false) {
      return false;
    }

    page_size_ = GetPageSize();
    if (page_size_ == 0) {
      return false;
    }

    mode_ = mode;

    state_ = STATE_OPENED;
    return true;
  }

  bool Close() {
    assert(state_ == STATE_OPENED || state_ == STATE_MAPPED);

    if (state_ == STATE_MAPPED) {
      bool rv = mapper_.Unmap();
      if (rv == false) {
        return false;
      }
      state_ = STATE_OPENED;
    }

    state_ = STATE_CLOSED;
    return true;
  }

  bool Get(size_t offset, char* output) {
    assert(state_ == STATE_OPENED || state_ == STATE_MAPPED);
    assert(mode_ == MODE_READ || mode_ == MODE_WRITE);
    assert(offset < file_.GetSize());

    size_t page_offset = offset / (page_size_ * pages_per_map) * pages_per_map;
    size_t data_offset = offset % (page_size_ * pages_per_map);

    bool rv = LoadChunk(page_offset);
    if (rv == false) {
      return false;
    }

    assert(data_offset < mapper_.GetSize());

    const char* data = mapper_.GetPointer();
    *output = data[data_offset];

    return true;
  }

  bool Set(size_t offset, char input) {
    assert(state_ == STATE_OPENED || state_ == STATE_MAPPED);
    assert(mode_ == MODE_WRITE);
    assert(offset < file_.GetSize());

    size_t page_offset = offset / (page_size_ * pages_per_map) * pages_per_map;
    bool rv = LoadChunk(page_offset);
    if (rv == false) {
      return false;
    }

    size_t data_offset = offset % (page_size_ * pages_per_map);
    assert(data_offset < mapper_.GetSize());
    char* data = mapper_.GetPointer();
    data[data_offset] = input;

    return true;
  }

  size_t GetSize() const {
    assert(state_ == STATE_OPENED || state_ == STATE_MAPPED);
    return file_.GetSize();
  }

  bool SetSize(size_t size) {
    assert(state_ == STATE_OPENED || state_ == STATE_MAPPED);
    if (file_.GetSize() == size) {
      return true;
    }
    if (state_ == STATE_MAPPED) {
      bool rv = mapper_.Unmap();
      if (rv == false) {
        return false;
      }
      state_ = STATE_OPENED;
    }
    bool rv = file_.SetSize(size);
    if (rv == false) {
      return false;
    }
    return true;
  }

 private:
  bool LoadChunk(size_t page_offset) {
    assert(state_ == STATE_OPENED || state_ == STATE_MAPPED);

    if (state_ == STATE_MAPPED) {
      if (page_offset == mapper_.GetPageOffset()) {
        return true;
      }
      bool rv = mapper_.Unmap();
      if (rv == false) {
        return false;
      }
      state_ = STATE_OPENED;
    }

    bool rv = mapper_.Map(page_offset, pages_per_map);
    if (rv == false) {
      return false;
    }

    state_ = STATE_MAPPED;
    return true;
  }

  File file_;
  FileMapper mapper_;

  size_t page_size_;

  static const size_t pages_per_map = 256; // Should be > 0.

  Mode mode_;
  enum {
    STATE_CLOSED,
    STATE_OPENED,
    STATE_MAPPED
  } state_;
};

#endif // MAPPED_FILE_HPP_
