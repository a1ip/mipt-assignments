#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include <algorithm>
#include <vector>

#include "thread_pool.hpp"

// Executes the 'function' with the argument 'arg' in 'Run' method.
class FunctionTask : public WaitableTask {
 public:
  FunctionTask(void (*function)(void*), void* arg) : function_(function), arg_(arg) { }
  ~FunctionTask() { }

  virtual void Run() {
    function_(arg_);
  }

 private:
  void (*function_)(void*);
  void* arg_;
};

struct merge_sort_argument {
  int* data;
  int* buffer;
  ThreadPool* pool;
  size_t left;
  size_t right;
  int depth;

  merge_sort_argument(int* data, int* buffer, ThreadPool* pool, size_t left, size_t right, int depth)
    : data(data), buffer(buffer), pool(pool), left(left), right(right), depth(depth) { }
};

void merge_sort_impl(void* raw_arg) {
  assert(raw_arg != NULL);

  merge_sort_argument* arg = reinterpret_cast<merge_sort_argument*>(raw_arg);
  int* data = arg->data;
  int* buffer = arg->buffer;
  ThreadPool* pool = arg->pool;
  size_t left = arg->left;
  size_t right = arg->right;
  int depth = arg->depth;

  assert(data != NULL);
  assert(buffer != NULL);
  assert(pool != NULL);
  assert(right >= left);
  
  size_t length = right - left + 1;
  if(length == 1) {
    return;
  }

  size_t middle = length / 2;
  int child_depth = depth - 1 > 0 ? depth - 1 : 0;
  merge_sort_argument left_arg(data, buffer, pool, left, left + middle - 1, child_depth);
  merge_sort_argument right_arg(data, buffer, pool, left + middle, right, child_depth);

  if(depth > 0) {
    FunctionTask left_task(&merge_sort_impl, &left_arg);
    pool->PostTask(&left_task);

    FunctionTask right_task(&merge_sort_impl, &right_arg);
    pool->PostTask(&right_task);

    left_task.Wait();
    right_task.Wait();
  } else {
    merge_sort_impl(&left_arg);
    merge_sort_impl(&right_arg);
  }

  size_t first = left;
  size_t second = left + middle;
  for(size_t i = left; i <= right; i++) {
    if(first < left + middle && second <= right) {
      if(data[first] < data[second]) {
        buffer[i] = data[first];
        first++;
      } else {
        buffer[i] = data[second];
        second++;
      }
    } else {
      if(first < left + middle) {
        buffer[i] = data[first];
        first++;
      } else {
        buffer[i] = data[second];
        second++;
      }
    }
  }

  memcpy(data + left, buffer + left, length * sizeof(int));
}

void merge_sort(int* data, size_t size, int depth) {
  assert(depth >= 0);
  assert(depth <= 4);

  // num_of_workers = 2 ** (depth + 1) - 2;
  size_t num_workers = 1;
  for(size_t i = 0; i < depth + 1; i++) {
    num_workers *= 2;
  }
  num_workers -= 2;

  printf("Threads number: %u\n", num_workers + 1);

  ThreadPool pool(num_workers);
  pool.Start();

  int* buffer = new int[size];
  assert(buffer != NULL);

  merge_sort_argument arg(data, buffer, &pool, 0, size - 1, depth);
  merge_sort_impl(&arg);

  delete[] buffer;

  pool.ShutDown();
}

// Fills 'data' with random integers.
void fill_random(int* data, size_t size) {
  for(size_t i = 0; i < size; i++) {
    data[i] = rand();
  }
}

// Checks if the 'data' is sorted in ascending order.
bool check_sorted(int* data, size_t size) {
  for(size_t i = 1; i < size; i++) {
    if(data[i - 1] > data[i]) {
      return false;
    }
  }
  return true;
}

class Timer {
 public:
  Timer() {
    int rv = gettimeofday(&start_, NULL);
    assert(rv != -1);
  }
  ~Timer() { }

  // Returns the time since the creation in seconds.
  double Time() const {
    timeval now;
    int rv = gettimeofday(&now, NULL);
    assert(rv != -1);
    return double(now.tv_usec - start_.tv_usec) / 1000000 + now.tv_sec - start_.tv_sec;
  }

 private:
  timeval start_;
};

bool parse_int(char* input, int* output) {
  assert(input != NULL);
  assert(output != NULL);

  char* wrong_token = NULL;
  int time = strtol(input, &wrong_token, 10);
  if(*wrong_token != '\0') {
    return false;
  }
  *output = time;
  return true;
}

int main(int argc, char** argv) {
  assert(argc == 2);
  
  int depth;
  bool rv = parse_int(argv[1], &depth);
  assert(rv == true);
  assert(depth >= 0);

  const int size = 4 * 1024 * 1024;

  std::vector<int> data(size);

  fill_random(&data[0], size);

  Timer timer;

  merge_sort(&data[0], size, depth);
  //std::sort(data.begin(), data.end());

  printf("Time: %lf\n", timer.Time());

  assert(check_sorted(&data[0], size));

  return EXIT_SUCCESS;
}

