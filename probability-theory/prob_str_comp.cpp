#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include <string>

typedef long long int64_t;

// Returns the approximate number of prime numbers from '2' to 'k'.
double ApproximatePrimesCount(int64_t k) {
  assert(k >= 2);
  return static_cast<double>(k) / log(static_cast<double>(k));
}

// Returns a random number from '[0.0, 1.0]' with uniform distribution.
double RandomUniformFromZeroToOne() {
  return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

// Returns 'true', if 'k' is a prime number, returns 'false' otherwise.
bool IsPrime(int64_t k) {
  if(k < 2) {
    return false;
  }
  for(int64_t i = 2; i * i <= k; i++) {
    if(k % i == 0) {
      return false;
    }
  }
  return true;
}

// Returns the nearest prime number to 'k'.
int64_t FindNearestPrime(int64_t k) {
  int64_t i = 0;
  while(true) {
    if(IsPrime(k - i)) return k - i;
    if(IsPrime(k + i)) return k + i;
    i++;
  }
}

bool Compare(int64_t a, int64_t b, size_t n) {
  double N = n * n * n;
  double r = RandomUniformFromZeroToOne();
  double q = r * N / log(N);

  // Find integer 'k', that satisfies 'k / log(k) = q' most precisely.
  int64_t k = 3;
  while(true) {
    double previous_difference = fabs(ApproximatePrimesCount(k - 1) - q);
    double current_difference = fabs(ApproximatePrimesCount(k) - q);
    double next_difference = fabs(ApproximatePrimesCount(k + 1) - q);
    if(previous_difference >= current_difference && next_difference >= current_difference) {
      break;
    }
    k++;
  }

  int64_t p = FindNearestPrime(k);

  if(a % p != b % p) {
    return false;
  }
  return true;
}

int64_t BinaryStringToInt(const std::string& string) {
  assert(string.size() <= 64);

  size_t size = string.size();
  int64_t result = 0;

  for(size_t i = 0; i < size; i++) {
    assert(string[i] == '0' || string[i] == '1');
    result <<= 1;
    if(string[i] == '1') {
      result += 1;
    }
  }

  return result;
}

bool Compare(const std::string& first, const std::string& second) {
  int64_t a = BinaryStringToInt(first);
  int64_t b = BinaryStringToInt(second);
  int64_t n = std::max(first.size(), second.size());
  return Compare(a, b, n);
} 

int main() {
  std::string first = "101111000110000101001110";
  std::string second = "101111000110000101001110";
  int attempts = 100;

  int equal = 0;
  int not_equal = 0;
  for(int i = 0; i < attempts; i++) {
    if(Compare(first, second)) {
      equal++;
    } else {
      not_equal++;
    }
  }
  printf("%s == %s: %d\n", first.c_str(), second.c_str(), equal);
  printf("%s != %s: %d\n", first.c_str(), second.c_str(), not_equal);

  return 0;
}

