#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <vector>

#include <sys/time.h>

#include <omp.h>

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

  size_t middle = left + size / 2;
  size_t first_left = left;
  size_t first_right = middle;
  size_t second_left = middle;
  size_t second_right = right;

  MergeSortImpl(data, buffer, first_left, first_right);
  MergeSortImpl(data, buffer, second_left, second_right);

  std::copy(data + left, data + right, buffer + left);
  Merge(data, buffer, left, middle, right);
}

void MergeSort(std::vector<int>& array) {
  std::vector<int> buffer(array.size());
  MergeSortImpl(&array[0], &buffer[0], 0, array.size());
}

int main(int argc, char** argv) {
  const int array_size = 128 * 1024 * 1024 + 17;
  const int threads = 5;

  std::vector<int> array;
  array.reserve(array_size);
  std::generate_n(std::back_inserter(array), array_size, std::rand);
  std::vector<int> buffer(array_size, 0);
  struct {
    int left, right;
  } partner_bounds[threads];

  Timer timer;

  #pragma omp parallel shared(array, buffer, partner_bounds) num_threads(threads)
  {
    int tid = omp_get_thread_num();
    int left = array_size / threads * tid + std::min(array_size % threads, tid);
    int right = left + array_size / threads + (tid < array_size % threads ? 1 : 0);
    //printf("%d %d %d\n", tid, left, right);

    MergeSortImpl(&array[0], &buffer[0], left, right);
    for (int i = left; i < right - 1; i++) {
      assert(array[i] <= array[i + 1]);
    }

    #pragma omp barrier

    bool done = false;
    for (int i = 0; i < sizeof(int) * 8 / 2; i++) {
      int partner_tid = tid ^ (1 << i);
      if (done || partner_tid >= threads) {
        #pragma omp barrier
        #pragma omp barrier
        continue;
      }
      if ((tid & (1 << i)) == 0) {
        #pragma omp barrier
        int partner_left = partner_bounds[partner_tid].left;
        int partner_right = partner_bounds[partner_tid].right;
        //printf("%d | %d %d || %d | %d %d\n", tid, left, right, partner_tid, partner_left, partner_right);
        assert(right == partner_left);

        std::copy(&array[0] + left, &array[0] + partner_right, &buffer[0] + left);
        Merge(&array[0], &buffer[0], left, right, partner_right);
        right = partner_right;
        #pragma omp barrier
      } else {
        partner_bounds[tid].left = left;
        partner_bounds[tid].right = right;
        done = true;
        #pragma omp barrier
        #pragma omp barrier
      }
    }

    #pragma omp barrier
  }

  std::cout << threads << " " << timer.GetTime() << std::endl;
  for (int i = 0; i < array_size - 1; i++) {
    assert(array[i] <= array[i + 1]);
  }
}
