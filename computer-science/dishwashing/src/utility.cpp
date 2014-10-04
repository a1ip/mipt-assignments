#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

#include "utility.hpp"

void Split(const std::string& source, const std::string& delimeters, std::vector<std::string>* output) {
  output->clear();
  size_t read_till = 0;
  while(true) {
    size_t current_entry = source.find_first_of(delimeters, read_till);
    if(current_entry == std::string::npos)
      break;
    if(read_till != current_entry)
      output->push_back(source.substr(read_till, current_entry - read_till));
    read_till = current_entry + 1;
  }
  if(read_till != source.size())
    output->push_back(source.substr(read_till, source.size() - read_till));
}

bool ReadFileToString(const std::string& filename, std::string* output) {  
  FILE* file = fopen(filename.c_str(), "r");
  if(file == NULL) {
    return false;
  }

  output->clear();
  size_t buffer_size = 16;
  std::vector<char> buffer(buffer_size);
  while(true) {
    size_t bytes_read = fread(&buffer[0], sizeof(char), buffer_size, file);
    output->append(&buffer[0], bytes_read);
    if(bytes_read < buffer_size)
      break;
  }

  if(ferror(file) != 0) {
    output->clear();
    return false;
  }
  if(fclose(file) != 0) {
    output->clear();
    return false;
  }

  return true;
}

bool ParseInt(const std::string& input, int* output) {
  char* wrong_token = NULL;
  int time = strtol(input.c_str(), &wrong_token, 10);
  if(*wrong_token != '\0') {
    return false;
  }
  *output = time;
  return true;
}


