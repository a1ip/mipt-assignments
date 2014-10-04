#ifndef FILE_MAPPER_HPP_
#define FILE_MAPPER_HPP_

#include <assert.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "file.hpp"
#include "logger.hpp"
#include "utility.hpp"

// Maps in memory the part of file. Starting address is a multiple of the size
// of one page. The first page and the number of pages of the file to be mapped
// should be specified. If the size of the file is less than the size of the
// part of the file one wants to be mapped, that only the actually existing
// data will be mapped.
class FileMapper {
 public:
  enum Mode {
    MODE_READ,
    MODE_WRITE
  };

  FileMapper() : state_(STATE_CLOSED) { }
  ~FileMapper() {
    if (state_ == STATE_MAPPED) {
      Unmap();
    }
  }

  bool Open(File* file, Mode mode) {
    assert(state_ == STATE_CLOSED);
    assert(file != NULL);

    file_ = file;

    page_size_ = GetPageSize();
    if (page_size_ == 0) {
      return false;
    }

    mode_ = mode;

    state_ = STATE_OPENED;
    return true;
  }

  bool Map(size_t page_offset, size_t pages_count) {
    assert(state_ == STATE_OPENED || state_ == STATE_MAPPED);
    assert(pages_count > 0);

    /*if (state_ == STATE_MAPPED && page_offset_ == page_offset
        && pages_count_ == pages_count && mapping_size_ == mapping_size) {
      return true;
    }*/

    if (state_ == STATE_MAPPED) {
      bool rv = Unmap();
      if (rv == false) {
        return false;
      }
    }

    page_offset_ = page_offset;
    pages_count_ = pages_count;

    size_t file_size = file_->GetSize();
    size_t offset = page_offset_ * page_size_;
    if (offset >= file_size) {
      Logger::GetInstance().Log(Logger::LEVEL_ERROR,
        "Couldn't map file chunk, file is too small.");
      return false;
    }
    mapping_size_ = std::min(pages_count * page_size_, file_size - offset);

    int prot;
    if (mode_ == MODE_READ) {
      prot = PROT_READ;
    } else {
      prot = PROT_READ | PROT_WRITE;
    }
    mapping_pointer_ = static_cast<char*>(mmap(NULL, mapping_size_, prot,
      MAP_SHARED, file_->GetFD(), offset));
    if (mapping_pointer_ == MAP_FAILED) {
      Logger::GetInstance().Log(Logger::LEVEL_ERROR,
        "Couldn't map file '%d' to memory.", file_->GetFD());
      return false;
    }

    state_ = STATE_MAPPED;
    return true;
  }

  bool Unmap() {
    assert(state_ == STATE_MAPPED);

    int rv = munmap(mapping_pointer_, mapping_size_);
    if (rv != 0) {
      Logger::GetInstance().Log(Logger::LEVEL_ERROR,
        "Couldn't unmap file with descriptor %d.", file_->GetFD());
      state_ = STATE_CLOSED;
      return false;
    }

    state_ = STATE_OPENED;
    return true;
  }

  size_t GetSize() const {
    assert(state_ == STATE_OPENED || state_ == STATE_MAPPED);
    return mapping_size_;
  }

  char* GetPointer() {
    assert(state_ == STATE_MAPPED);
    return mapping_pointer_;
  }

  size_t GetPageOffset() const {
    assert(state_ == STATE_MAPPED);
    return page_offset_;
  }

 private:
  File* file_;

  size_t page_size_;

  size_t page_offset_;
  size_t pages_count_;

  size_t mapping_size_;
  char* mapping_pointer_;

  Mode mode_;
  enum {
    STATE_CLOSED,
    STATE_OPENED,
    STATE_MAPPED
  } state_;  
};

#endif // FILE_MAPPER_HPP_
