#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>

void PrintUsage(int argc, char** argv) {
  std::cout << "Usage:" << std::endl;
  std::cout << std::endl;

  std::cout << argv[0] << " " << "N" << std::endl;
  std::cout << "N is the number of integers that will be read from input. " <<
    std::endl << "N should be positive integer." << std::endl;
  std::cout << std::endl;

  std::cout << "Input integers should be <= than " <<
    std::numeric_limits<int32_t>::max() << " and >= than " <<
    std::numeric_limits<int32_t>::min() << "." << std::endl;
  std::cout << "The integers should be whitespace separated." << std::endl;
}

bool ParseInt(const char* input, int32_t* output) {
  char* wrong_token = NULL;
  int32_t parsed_value =
    static_cast<int32_t>(strtol(input, &wrong_token, 10));
  if(*wrong_token != '\0' || errno == ERANGE) {
    return false;
  }
  *output = parsed_value;
  return true;
}

bool IsDivisibleBy3(int32_t n) {
  if (n < 0) {
    n = -n;
  }

  if (n == 0) {
    return true;
  }
  if (n == 1) {
    return false;
  }

  // If the alternating sum of the bits of the number is
  // a multiple of 3 then the number is divisible by 3.

  int32_t alternating_sum_of_bits = 0;

  size_t i = 0;
  while (n != 0) {
    if ((i & 1) == 0) {
      alternating_sum_of_bits += n & 1;
    } else {
      alternating_sum_of_bits -= n & 1;
    }
    n >>= 1;
    i += 1;
  }
  return IsDivisibleBy3(alternating_sum_of_bits);
}

bool IsDivisibleBy5(int32_t n) {
  if (n < 0) {
    n = -n;
  }

  if (n == 0) {
    return true;
  }
  if (n == 1 | n == 2 | n == 3) {
    return false;
  }

  // If the alternating sum of the even bits of the number plus
  // the alternating sum of the odd bits of the number multiplied
  // by 2 is a multiple of 5 then the number is divisible by 5.

  // 'sum_of_bits[0]' is the alternating sum of bits n[0], n[2], n[4], ...
  // 'sum_of_bits[1]' is the alternating sum of bits n[1], n[3], n[5], ...
  int32_t sum_of_bits[2] = { 0, 0 };

  size_t i = 0;
  while (n != 0) {
    if ((i & 2) == 0) {
      sum_of_bits[i & 1] += n & 1;
    } else {
      sum_of_bits[i & 1] -= n & 1;
    }
    n >>= 1;
    i += 1;
  }
  return IsDivisibleBy5(sum_of_bits[0] + sum_of_bits[1] * 2);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    PrintUsage(argc, argv);
    return EXIT_FAILURE;
  }

  int32_t input_size;
  if (!ParseInt(argv[1], &input_size) || input_size <= 0) {
    PrintUsage(argc, argv);
    return EXIT_FAILURE;
  }

  for (int32_t i = 0; i < input_size; i++) {
    std::string input;
    std::cin >> input;

    int32_t current_number;
    if (!ParseInt(input.c_str(), &current_number)) {
      std::cerr << "Error: input integers should be <= than " <<
        std::numeric_limits<int32_t>::max() << " and >= than " <<
        std::numeric_limits<int32_t>::min() << "." << std::endl;
      return EXIT_FAILURE;
    }

    bool is_divisible_by_3 = IsDivisibleBy3(current_number);
    bool is_divisible_by_5 = IsDivisibleBy5(current_number);

    if (is_divisible_by_3) {
      std::cout << "bizz";
    }
    if (is_divisible_by_5) {
      std::cout << "buzz";
    }
    if (!is_divisible_by_3 && !is_divisible_by_5) {
      std::cout << current_number;
    }
    std::cout << " ";
  }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
