#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>

#include "thread.hpp"
#include "semaphore.hpp"

double from = 0.0;
double to = 1.0;
double eps = 0.001;

double result = 0.0;
Semaphore semaphore;

double f(double x) {
  return 4.0 / (1 + x * x);
}

struct IntegrateArgument {
  IntegrateArgument(size_t offset, size_t steps, double step_size)
    : offset(offset), steps(steps), step_size(step_size) { }

  size_t offset;
  size_t steps;
  double step_size;
};

void Integrate(void* arg) {
  assert(arg != NULL);
  IntegrateArgument* argument = reinterpret_cast<IntegrateArgument*>(arg);

  double local_result = 0.0;
  for (size_t i = 0; i < argument->steps; i++) {
    double left = (argument->offset + i) * argument->step_size;
    double right = (argument->offset + i + 1) * argument->step_size;
    local_result += argument->step_size * (f(left) + f(right)) / 2.0;
  }

  bool rv = semaphore.Wait();
  assert(rv == true);
  result += local_result;
  rv = semaphore.Signal();
  assert(rv == true);

  delete argument;
}

int main(int argc, char** argv) {
  assert(argc == 2);

  int workers = atoi(argv[1]);
  assert(workers > 0);

  std::vector<Thread*> threads;
  threads.reserve(static_cast<size_t>(workers));

  bool rv = semaphore.Initialize(1);
  assert(rv == true);

  size_t steps = static_cast<size_t>(sqrt(40.0 * (to - from) * (to - from) * (to - from) / 12.0 / eps)) + 1;
  printf("Steps: %lu\n", steps);
  double step_size = (to - from) / steps;

  size_t offset = 0;
  for (int i = 0; i < workers; i++) {
    size_t worker_steps = steps / workers;
    if (i < steps % workers) {
      worker_steps++;
    }

    IntegrateArgument* arg = new IntegrateArgument(offset, worker_steps, step_size);
    assert(arg != NULL);

    offset += worker_steps;

    Thread* thread = new Thread();
    assert(thread != NULL);
    bool rv = thread->Start(&Integrate, arg);
    assert(rv == true);
    threads.push_back(thread);
  }

  for (int i = 0; i < workers; i++) {
    threads[static_cast<size_t>(i)]->Join();
    delete threads[static_cast<size_t>(i)];
  }

  rv = semaphore.Finalize();
  assert(rv == true);

  printf("Result: %lf\n", result);

  return EXIT_SUCCESS;
}
