#ifndef CSV_READER_HPP_
#define CSV_READER_HPP_

#include <string.h>

#include <string>

#include "file.hpp"
#include "logger.hpp"
#include "mapped_file.hpp"
#include "utility.hpp"

class CSVReader {
 public:
  CSVReader() : state_(STATE_CLOSED) { }
  ~CSVReader() {
    if (state_ == STATE_OPENED) {
      Close();
      state_ = STATE_CLOSED;
    }
  }

  bool Open(const char* file_name) {
    assert(state_ == STATE_CLOSED);

    bool rv = mapped_file_.Open(file_name, MappedFile::MODE_READ);
    if (rv == false) {
      return false;
    }
    position_ = 0;
    has_next_ = false;

    state_ = STATE_OPENED;
    return true;
  }

  bool Close() {
    assert(state_ == STATE_OPENED);

    bool rv = mapped_file_.Close();
    if (rv == false) {
      return false;
    }

    state_ = STATE_CLOSED;
    return true;
  }

  // You should ensure that there is a number to read using
  // 'HasNext' before reading.
  int32_t ReadNext() {
    assert(state_ == STATE_OPENED);
    assert(has_next_ == true);
    has_next_ = false;
    return next_;
  }

  // Returns 'true' on success and stores the result in 'has_next'.
  // Returns 'false' on error or if the end of the file is reached.
  bool HasNext(bool* has_next) {
    assert(state_ == STATE_OPENED);
    assert(has_next != NULL);
    if (has_next_) {
      *has_next = true;
      return true;
    }
    bool rv = TryParseNext();
    if (rv == false) {
      return false;
    }
    *has_next = has_next_;
    return true;
  }

 private:
  bool TryParseNext() {
    assert(state_ == STATE_OPENED);
    assert(has_next_ == false);

    size_t file_size = mapped_file_.GetSize();
    std::string current_string;

    while (position_ < file_size) {
      char current_char;
      bool rv = mapped_file_.Get(position_, &current_char);
      if (rv == false) {
        return false;
      }

      if (isdigit(current_char) != 0 || current_char == '-') {
        current_string.push_back(current_char);
      } else if (isspace(current_char) != 0 || current_char == ',') {
        if (current_string.size() > 0) {
          bool is_valid = ParseInt(current_string.c_str(), &next_); 
          if (is_valid) {
            has_next_ = true;
            return true;
          } else {
            Logger::GetInstance().Log(Logger::LEVEL_ERROR,
              "Couldn't parse int while reading from CSV file.");
            return false;
          }
          current_string.resize(0);
        }
      } else {
        Logger::GetInstance().Log(Logger::LEVEL_ERROR,
          "Met unexpected character '%c' while reading from CSV file.",
          current_char);
        return false;
      }

      position_ += 1;
    }

    has_next_ = false;
    return true;
  }

  MappedFile mapped_file_;
  size_t position_;

  bool has_next_;
  int32_t next_;

  enum {
    STATE_CLOSED,
    STATE_OPENED
  } state_;
};

#endif // CSV_READER_HPP_
