#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <vector>

#include <sys/time.h>

class Timer {
 public:
  Timer() {
    int rv = gettimeofday(&_start, NULL);
    assert(rv == 0);
  }

  int64_t GetTime() const {
    timeval current;
    int rv = gettimeofday(&current, NULL);
    assert(rv == 0);
    int64_t seconds = current.tv_sec - _start.tv_sec;
    int64_t useconds = current.tv_usec - _start.tv_usec;
    int64_t time = seconds * 1000 + useconds / 1000;
    return time;
  }

 private:
  timeval _start;
};

void Merge(int* data, int* buffer, size_t left, size_t middle, size_t right) {
  size_t first_left = left;
  size_t first_right = middle;
  size_t second_left = middle;
  size_t second_right = right;

  size_t first = first_left;
  size_t second = second_left;

  size_t current = left;

  while (first < first_right || second < second_right) {
    if (first < first_right && second == second_right) {
      data[current] = buffer[first];
      first++;
    } else if (first == first_right && second < second_right) {
      data[current] = buffer[second];
      second++;
    } else {
      if (buffer[first] < buffer[second]) {
        data[current] = buffer[first];
        first++;
      } else {
        data[current] = buffer[second];
        second++;
      }
    }
    current++;
  }
}

void MergeSortImpl(int* data, int* buffer, size_t left, size_t right) {
  assert(right >= left);

  size_t size = right - left;

  if (size <= 1) {
    return;
  }

  size_t first_left = left;
  size_t first_right = left + size / 2;
  size_t second_left = left + size / 2;
  size_t second_right = right;

  MergeSortImpl(data, buffer, first_left, first_right);
  MergeSortImpl(data, buffer, second_left, second_right);

  std::copy(data + left, data + right, buffer + left);
  Merge(data, buffer, left, second_left, right);
}

void MergeSort(std::vector<int>& array) {
  std::vector<int> buffer(array.size());
  MergeSortImpl(&array[0], &buffer[0], 0, array.size());
}

int main() {
  std::vector<int> array;
  for (size_t i = 1024 * 1024; i <= 32 * 1024 * 1024; i += 1024 * 1024) {
    array.clear();
    array.reserve(i);
    std::generate_n(std::back_inserter(array), i, std::rand);
    Timer timer;
    MergeSort(array);
    std::cout << i << " " << timer.GetTime() << std::endl;
  }
}
