#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
     
#include <string>
#include <vector>

#include "utility.hpp"

ScopedFile::ScopedFile(FILE* file) : file_(file) { }

ScopedFile::~ScopedFile() {
  if(file_ != NULL) {
    fclose(file_);
  }
}

bool GetFileSize(const std::string& filename, size_t* output) {
  struct stat file_status;
  if(stat(filename.c_str(), &file_status) != 0) {
    *output = 0;
    return true;
  }
  *output = file_status.st_size;
  return true;
}

bool StrToInt(const std::string& input, int* output) {
  char* wrong_token = NULL;
  int time = strtol(input.c_str(), &wrong_token, 10);
  if(*wrong_token != '\0') {
    return false;
  }
  *output = time;
  return true;
}

bool GetSHA1Hash(const char* data, size_t size, std::string* output) {
  output->clear();

  unsigned char hash[SHA_DIGEST_LENGTH];
  if(SHA1(reinterpret_cast<const unsigned char*>(data), size, hash) == NULL) {
    return false;
  }

  char hex[3];
  for(size_t i = 0; i < SHA_DIGEST_LENGTH; i++) {
    if(sprintf(hex, "%02x", hash[i]) < 0) {
      output->clear();
      return false;
    }
    hex[2] = '\0';
    output->append(hex);
  }

  return true;
}

