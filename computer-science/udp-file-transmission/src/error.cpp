#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <string>

#include "error.hpp"

namespace error {

ErrorType last_error = ERROR_ERRNO;

void SetLastError(ErrorType error) {
  last_error = error;
}

ErrorType GetLastError() {
  return last_error;
}

std::string GetDescription(ErrorType error) {
  switch(error) {
    case ERROR_ERRNO:
      return std::string("In 'errno': \"") + strerror(errno) + "\".";
    case ERROR_TCP_SOCKET_CLOSED:
      return "Error: TCP socket was closed while there was data to transmit!";
    default:
      return "Error: unknown error.";
  }
}

void PrintLastErrorDescription() {
  fprintf(stderr, "%s\n", GetDescription(GetLastError()).c_str());
}

} /*namespace error*/

