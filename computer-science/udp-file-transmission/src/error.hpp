#ifndef UDP_FILE_TRANSMISSION_ERROR_HPP_
#define UDP_FILE_TRANSMISSION_ERROR_HPP_

#include <string>

namespace error {

enum ErrorType {
  ERROR_ERRNO = 0,
  ERROR_TCP_SOCKET_CLOSED,
};

void SetLastError(ErrorType error);

ErrorType GetLastError();

std::string GetDescription(ErrorType error);

void PrintLastErrorDescription();

} /*namespace error*/

#endif /*UDP_FILE_TRANSMISSION_ERROR_HPP_*/

