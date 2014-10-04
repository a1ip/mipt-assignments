#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <vector>

#include <sys/time.h>

#include <mpi.h>

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
  MPI_Init(&argc, &argv);

  MPI_Status status;
  int size, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  assert(size > 0);

  const int total_array_size = 128 * 1024 * 1024 + 17;
  int array_size;
  std::vector<int> array;
  std::vector<int> buffer;

  Timer timer;

  if (rank == 0) {
    array.reserve(total_array_size);
    std::generate_n(std::back_inserter(array), total_array_size, std::rand);

    array_size = total_array_size / size;
    if (total_array_size % size > 0) {
      array_size++;
    }

    int worker_part_beg = array_size;
    for (int worker = 1; worker < size; worker++) {
      int worker_part_size = total_array_size / size;
      if (total_array_size % size > worker) {
        worker_part_size++;
      }
      MPI_Send(&worker_part_size, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
      MPI_Send(&array[worker_part_beg], worker_part_size, MPI_INT, worker, 1, MPI_COMM_WORLD);
      worker_part_beg += worker_part_size;
    }
    assert(worker_part_beg == total_array_size);

    array.resize(array_size);
  } else {
      MPI_Recv(&array_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      array.resize(array_size);
      MPI_Recv(&array[0], array_size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
  }

  MergeSort(array);

  for (int i = 0; i < sizeof(int) * 8 / 2; i++) {
    int partner = rank ^ (1 << i);
    if (partner >= size) {
      continue;
    }
    if ((rank & (1 << i)) == 0) {
      int partner_array_size;
      MPI_Recv(&partner_array_size, 1, MPI_INT, partner, 2, MPI_COMM_WORLD, &status);
      array.resize(array_size + partner_array_size);
      MPI_Recv(&array[array_size], partner_array_size, MPI_INT, partner, 1, MPI_COMM_WORLD, &status);

      buffer.resize(array.size());
      std::copy(&array[0], &array[0] + array.size(), &buffer[0]);
      Merge(&array[0], &buffer[0], 0, array_size, array_size + partner_array_size);
      array_size += partner_array_size;
    } else {
      MPI_Send(&array_size, 1, MPI_INT, partner, 2, MPI_COMM_WORLD);
      MPI_Send(&array[0], array_size, MPI_INT, partner, 1, MPI_COMM_WORLD);
      break;
    }
  }

  if (rank == 0) {
    assert(array.size() == total_array_size);
    std::cout << "Time elapsed: " << timer.GetTime() << std::endl;
    for (int i = 0; i < total_array_size - 1; i++) {
      assert(array[i] <= array[i + 1]);
    }
  }

  MPI_Finalize();
}
