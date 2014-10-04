#include <assert.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <locale>

size_t CountEntries(const std::string& source, const std::string& word) {
  size_t entries_found = 0;
  size_t position = 0;
  while ((position = source.find(word, position)) != std::string::npos) {
    entries_found += 1;
    position += 1;
  }
  return entries_found;
}

int ToLower(int c) {
  if ((c >= 128) && (c <= 143))
    return (c + 32);
  else if ((c >= 144) && (c <= 159))
    return (c + 80);
  else
    return (tolower(c));
}

void Decapitilize(std::string& string) {
  for (size_t i = 0; i < string.size(); i++) {
    string[i] = ToLower(string[i]);
  }
}

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cout << "Usage: " << argv[0] << " " << "FILE..." << std::endl;
    std::cout << "This program count the number of entries of words 'radost' and 'радость' in the listed files. Case insensitive." << std::endl;
    return EXIT_FAILURE;
  }

  size_t entries_found = 0;

  for (int i = 1; i < argc; i++) {
    std::ifstream input;
    input.open(argv[i]);
    if (input.fail()) {
      std::cerr << "File '" << argv[i] << "' not found!" << std::endl;
      continue;
    }

    std::string current_line;
    while (input.good()) {
      std::getline(input, current_line);
      Decapitilize(current_line);
      entries_found += CountEntries(current_line, "joy");
      entries_found += CountEntries(current_line, "radost");
      entries_found += CountEntries(current_line, "радость");
    }
  }

  std::cout << "Joy entries: " << entries_found << std::endl;
  return EXIT_SUCCESS;
}
