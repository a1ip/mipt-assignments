#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/time.h>

#include "logger.hpp"

class Timer {
 public:
  Timer() {
    int rv = gettimeofday(&start_, NULL);
    assert(rv != -1);
  }
  ~Timer() { }

  // Returns the time since the creation in milliseconds.
  double Time() const {
    timeval now;
    int rv = gettimeofday(&now, NULL);
    assert(rv != -1);
    return (now.tv_usec - start_.tv_usec) / 1000.0 + (now.tv_sec - start_.tv_sec) * 1000.0;
  }

 private:
  timeval start_;
};

// Reads an integer from 'input' and stores in 'output'.
// Returns false on error, returns true on success.
bool ParseInt(const char* input, int32_t* output) {
  assert(input != NULL);
  assert(output != NULL);
  char* wrong_token = NULL;
  int32_t parsed_value =
    static_cast<int32_t>(strtol(input, &wrong_token, 10));
  if(*wrong_token != '\0' || errno == ERANGE) {
    Logger::GetInstance().Log(Logger::LEVEL_ERROR, "Couldn't parse int from '%s'.", input);
    return false;
  }
  *output = parsed_value;
  return true;
}

// Returns the size of a page in bytes on success.
// Returns '0' on error.
size_t GetPageSize() {
  long size = sysconf(_SC_PAGE_SIZE);
  if (size <= 0) {
    Logger::GetInstance().Log(Logger::LEVEL_ERROR, "Couldn't get page size.");
    return 0;
  }
  return static_cast<size_t>(size);
}

bool RemoveFile(const char* path) {
  int rv = remove(path);
  if (rv != 0) {
    Logger::GetInstance().Log(Logger::LEVEL_ERROR, "Couldn't remove file '%s'.", path);
    return false;
  }
  return true;
}

#endif // UTILITY_HPP_
