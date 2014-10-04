#ifndef MERGE_SORT_HPP_
#define MERGE_SORT_HPP_

#include <stdint.h>

#include "logger.hpp"
#include "file_storage.hpp"
#include "utility.hpp"

namespace {

struct MergeSortArg {
  MergeSortArg(FileStorage* output, FileStorage* buffer, size_t left, size_t right)
    : output(output), buffer(buffer), left(left), right(right) { }
  FileStorage* output;
  FileStorage* buffer;
  size_t left;
  size_t right;
};

bool MergeSortImpl(const MergeSortArg& arg) {
  size_t size = arg.right - arg.left;
  if (size <= 1) {
    return true;
  }

  size_t first_size = size / 2;
  size_t second_size = size - first_size;

  size_t first_left = arg.left;
  size_t first_right = arg.left + first_size;

  size_t second_left = arg.left + first_size;
  size_t second_right = arg.right;

  MergeSortArg first_arg(arg.buffer, arg.output, first_left, first_right);
  MergeSortArg second_arg(arg.buffer, arg.output, second_left, second_right);

  bool rv = MergeSortImpl(first_arg) && MergeSortImpl(second_arg);
  if (rv == false) {
    return false;
  }

  size_t first = 0;
  size_t second = 0;
  int32_t current;

  for (size_t i = 0; i < size; i++) {
    if (first == first_size) {
      rv = arg.buffer->Get(second_left + second++, &current)
        && arg.output->Set(arg.left + i, current);
      if (rv == false) {
        return false;
      }
      continue;
    }
    if (second == second_size) {
      rv = arg.buffer->Get(first_left + first++, &current)
        && arg.output->Set(arg.left + i, current);
      if (rv == false) {
        return false;
      }
      continue;
    }

    int32_t first_next, second_next;
    rv = arg.buffer->Get(first_left + first, &first_next)
      && arg.buffer->Get(second_left + second, &second_next);

    if (first_next <= second_next) {
      rv = arg.output->Set(arg.left + i, first_next);
      if (rv == false) {
        return false;
      }
      first++;
    } else {
      rv = arg.output->Set(arg.left + i, second_next);
      if (rv == false) {
        return false;
      }
      second++;
    }
  }

  return true;
}

} // Anonymous namespace.

bool MergeSort(FileStorage* input, FileStorage* output) {
  assert(input != NULL);
  assert(output != NULL);
  assert(input->GetSize() == output->GetSize());

  size_t size = input->GetSize();
  FileStorage buffer;
  bool rv = buffer.Open("merge_sort_buffer.dat", size);
  if (rv == false) {
    return false;
  }

  int32_t current;
  for (size_t i = 0; i < size; i++) {
    rv = input->Get(i, &current) && output->Set(i, current)
      && buffer.Set(i, current);
    if (rv == false) {
      return false;
    }
  }

  MergeSortArg arg(output, &buffer, 0, size);
  rv = MergeSortImpl(arg);
  if (rv == false) {
    return false;
  }

  rv = buffer.Close();
  if (rv == false) {
    return false;
  }

  rv = RemoveFile("merge_sort_buffer.dat");
  if (rv == false) {
    return false;
  }

  return true;
}

#endif // MERGE_SORT_HPP_
