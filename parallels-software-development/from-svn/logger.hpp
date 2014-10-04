#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <vector>

class Logger {
 public:
  enum Level {
    LEVEL_ERROR = 0,
    LEVEL_WARNING = 1,    
    LEVEL_INFO = 2
  };

  static Logger& GetInstance() {
    static Logger logger;
    return logger;
  }

  bool Initialize(const char* file) {
    assert(file_ == NULL);
    file_ = fopen(file, "w+");
    if (file_ == NULL) {
      return false;
    }
    message_buffer_.reserve(message_buffer_max_size);
    return true;
  }

  bool Finalize() {
    assert(file_ != NULL);
    Flush();
    int rv = fclose(file_);
    file_ = NULL;
    if (rv != 0) {
      return false;
    }
    return true;
  }

  void SetLevel(Level level) {
    level_ = level;
  }

  void Log(Level level, const char* format, ...) {
    assert(file_ != NULL);
    assert(format != NULL);

    if (static_cast<int>(level) > static_cast<int>(level_)) {
      return;
    }

    const size_t buffer_size = 1024; 
    char buffer[buffer_size];
    
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer) - 1, format, args);
    va_end(args);
    
    std::string entry = ConvertLevelToString(level);
    entry.append(": ");
    entry.append(buffer);
    message_buffer_.push_back(entry);

    if (message_buffer_.size() >= message_buffer_max_size) {
      Flush();
    }
  }

  void Flush() {
    assert(file_ != NULL);
    for (size_t i = 0; i < message_buffer_.size(); i++) {
      fprintf(file_, "%s\n", message_buffer_[i].c_str());
    }
    message_buffer_.clear();
  }

 private:
  Logger() : level_(LEVEL_WARNING), file_(NULL) { }
  ~Logger() {
    if (file_ != NULL) {
      Finalize();
    }
  }

  Logger(const Logger& other);
  Logger& operator = (const Logger& other);

  static std::string ConvertLevelToString(Level level) {
    if (level ==  LEVEL_INFO) {
      return "Info";
    } else if (level == LEVEL_WARNING) {
      return "Warning";
    } else if (level == LEVEL_ERROR) {
      return "Error";
    }
    assert(false); // Should not be reached.
  }

  static const size_t message_buffer_max_size = 32;
  std::vector<std::string> message_buffer_;

  Level level_;
  FILE* file_;
};

#endif // LOGGER_HPP_
