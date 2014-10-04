#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <string>
#include <vector>

// Splits the string 'source' using any of the characters that are part of 'delimeters'.
// Stores the result in 'output'.
void Split(const std::string& source, const std::string& delimeters, std::vector<std::string>* output);

// Reads the file to string, stores the result in 'output'.
// Returns true on success, returns false on error.
bool ReadFileToString(const std::string& filename, std::string* output);

// Reads an integer from a string.
// Returns true on success, returns false on error.
bool ParseInt(const std::string& input, int* output);

#endif /*_UTILITY_HPP*/

