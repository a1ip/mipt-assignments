#ifndef ERROR_HPP_
#define ERROR_HPP_

#include <string>

namespace error {

enum ErrorType {
  ERROR_ERRNO = 0,
  ERROR_MEMORY,
  ERROR_SEMAPHORE,
  ERROR_MSG_SIZE,
  ERROR_CONNECT,
  ERROR_IPC_TYPE,
};

void ReportError(ErrorType error);

ErrorType GetLastError();

std::string GetDescription(ErrorType error);

void PrintLastErrorDescription();

} /*namespace error*/

#endif /*ERROR_HPP_*/

