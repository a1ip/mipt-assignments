#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include <vector>

#include "thread.hpp"
#include "semaphore.hpp"

int total_sum = 0;
Semaphore semaphore;

struct Slice {
  Slice(int* data, size_t offset, size_t size)
    : data(data), offset(offset), size(size) { }

  int* data;
  size_t offset;
  size_t size;
};

void CalculateSum(void* arg) {
  assert(arg != NULL);
  Slice* slice = reinterpret_cast<Slice*>(arg);

  int sum = 0;
  for (size_t i = 0; i < slice->size; i++) {
    sum += slice->data[slice->offset + i];
  }

  printf("%d\n", sum);

  bool rv = semaphore.Wait();
  assert(rv == true);
  total_sum += sum;
  rv = semaphore.Signal();
  assert(rv == true);

  delete slice;
}

int main(int argc, char** argv) {
  assert(argc == 2);

  int workers = atoi(argv[1]);
  assert(workers > 0);

  std::vector<Thread*> threads;
  threads.reserve(static_cast<size_t>(workers));

  bool rv = semaphore.Initialize(1);
  assert(rv == true);

  const size_t array_size = 1000;
  int* array = new int[array_size];
  assert(array != NULL);
  for (size_t i = 0; i < array_size; i++) {
    array[i] = static_cast<int>(i);
  }

  size_t offset = 0;
  for (int i = 0; i < workers; i++) {
    size_t part_size = array_size / workers;
    if (i < array_size % workers) {
      part_size++;
    }

    Slice* slice = new Slice(array, offset, part_size);
    assert(slice != NULL);

    offset += part_size;

    Thread* thread = new Thread();
    assert(thread != NULL);
    bool rv = thread->Start(&CalculateSum, slice);
    assert(rv == true);
    threads.push_back(thread);
  }

  for (int i = 0; i < workers; i++) {
    threads[static_cast<size_t>(i)]->Join();
    delete threads[static_cast<size_t>(i)];
  }

  rv = semaphore.Finalize();
  assert(rv == true);

  printf("Result: %d\n", total_sum);

  return EXIT_SUCCESS;
}
