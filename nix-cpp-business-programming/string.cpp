#include <cassert>
#include <cstdlib>
#include <cstring>

#include <iostream>

class S {
 public:
  S() : data_(NULL), c_str_(NULL), length_(0) { }

  S(char c) {
    data_ = new char[1];
    assert(data_ != NULL);

    data_[0] = c;
    length_ = 1;
    c_str_ = NULL;
  }

  S(const char* string) {
    size_t length = strlen(string);
    if(length == 0) {
      data_ = NULL;
      length_ = 0;
    } else {
      data_ = new char[length];
      assert(data_ != NULL);

      memcpy(data_, string, length);
      length_ = length;
    }
    c_str_ = NULL;
  }

  S(const S& other) {
    if(other.length_ == 0) {
      data_ = NULL;
      length_ = 0;
    } else {
      data_ = new char[other.length_];
      assert(data_ != NULL);

      memcpy(data_, other.data_, other.length_);
      length_ = other.length_;
    }
    c_str_ = NULL;
  }

  S& operator = (const S& other) {
    if(this == &other) {
      return *this;
    }
    free();
    if(other.length_ == 0) {
      data_ = NULL;
      length_ = 0;
    } else {
      data_ = new char[other.length_];
      assert(data_ != NULL);

      memcpy(data_, other.data_, other.length_);
      length_ = other.length_;
    }
    return *this;
  }

  ~S() {
    free();
  }

  // For 'string_main.h'.
  size_t lenght() const {
    return length_;
  }

  size_t length() const {
    return length_;
  }

  const char* str() {
    if(c_str_ != NULL) {
      if(strlen(c_str_) != length_ || memcmp(c_str_, data_, length_) != 0) {
        delete [] c_str_;
        c_str_ = NULL;
      }
    }
    if(c_str_ == NULL) {
      c_str_ = new char[length_ + 1];
      assert(c_str_ != NULL);

      memcpy(c_str_, data_, length_);
      c_str_[length_] = '\0';
    }
    return c_str_;
  }

  const char* data() const {
    return data_;
  }

  char& operator [] (size_t i) {
    assert(i < length_);
    return data_[i];
  }

  const char& operator [] (size_t i) const {
    assert(i < length_);
    return data_[i];
  }

  S& operator += (const S& other) {
    if(other.length_ == 0) {
      return *this;
    }

    char* new_data = new char[length_ + other.length_];
    assert(new_data != NULL);

    memcpy(new_data, data_, length_);
    memcpy(new_data + length_, other.data_, other.length_);
    size_t new_length = length_ + other.length_;

    free();

    data_ = new_data;
    length_ = new_length;

    return *this;
  }

 private:
  void free() {
    if(data_ != NULL) {
      delete [] data_;
      data_ = NULL;      
    }
    if(c_str_ != NULL) {
      delete [] c_str_;
      c_str_ = NULL;
    }
    length_ = 0;
  }

  char* data_;
  char* c_str_;
  size_t length_;
};

S operator + (const S& first, const S& second) {
  S result(first);
  result += second;
  return result;
}

bool operator == (const S& first, const S& second) {
  if(first.length() != second.length()) {
    return false;
  }
  if(first.length() == 0) {
    return true;
  }
  if(memcmp(first.data(), second.data(), first.length()) != 0) {
    return false;
  }
  return true;
}

bool operator != (const S& first, const S& second) {
  return !(first == second);
}

std::ostream& operator << (std::ostream& output, const S& string) {
  if(string.length() > 0) {
    output.write(string.data(), string.length());
  }
  return output;
}

using namespace std;
#include <string_main.h>
