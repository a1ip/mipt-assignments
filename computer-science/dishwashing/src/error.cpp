#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <string>

#include "error.hpp"

namespace error {

ErrorType last_error = ERROR_ERRNO;

void ReportError(ErrorType error) {
  last_error = error;
}

ErrorType GetLastError() {
  return last_error;
}

std::string GetDescription(ErrorType error) {
  switch(error) {
    case ERROR_ERRNO:
      return std::string("In 'errno': \"") + strerror(errno) + "\".";
    case ERROR_MEMORY:
      return "Error: could not allocate memory!";
    case ERROR_SEMAPHORE:
      return "Error: semaphore failed!";
    case ERROR_MSG_SIZE:
      return "Error: message is too long!";
    case ERROR_CONNECT:
      return "Error: could not establish connection!";
    case ERROR_IPC_TYPE:
      return "Error: no such type of ipc!";
    default:
      return "Error: unknown error.";
  }
}

void PrintLastErrorDescription() {
  fprintf(stderr, "%s\n", GetDescription(GetLastError()).c_str());
}

} /*namespace error*/

