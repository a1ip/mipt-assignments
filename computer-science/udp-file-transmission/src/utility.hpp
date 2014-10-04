#ifndef UDP_FILE_TRANSMISSION_UTILITY_HPP_
#define UDP_FILE_TRANSMISSION_UTILITY_HPP_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>
#include <vector>

typedef long long int64;
typedef unsigned long long uint64;

class ScopedFile {
 public:
  explicit ScopedFile(FILE* file);
  ~ScopedFile();

 private:
  FILE* file_;
};

bool GetFileSize(const std::string& filename, size_t* output);

bool StrToInt(const std::string& input, int* output);

bool GetSHA1Hash(const char* data, size_t size, std::string* output);

#endif /*UDP_FILE_TRANSMISSION_UTILITY_HPP_*/

