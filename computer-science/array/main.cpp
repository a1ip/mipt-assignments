#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <iterator>

#include "array.hpp"

void run_tests() {
  Array<int> array;
  assert(array.Size() == 0);
  assert(array.IsEmpty() == true);

  assert(array.Begin() == array.End());

  array.PushBack(111);
  assert(array.Size() == 1);
  assert(array.IsEmpty() == false);
  assert(array[0] == 111);

  array.Resize(3, 222);
  assert(array.Size() == 3);
  assert(array.IsEmpty() == false);
  assert(array[0] == 111);
  assert(array[1] == 222);
  assert(array[2] == 222);

  array[1] = 333;
  assert(array[1] == 333);

  array.Resize(2, 444);
  assert(array.Size() == 2);
  assert(array.IsEmpty() == false);
  assert(array[0] == 111);
  assert(array[1] == 333);

  array.Resize(5, 555);
  int data[] = {111, 333, 555, 555, 555};
  size_t i = 0;

  Array<int>::Iterator itr;
  for(itr = array.Begin(); itr != array.End(); ++itr) {
    assert(*itr == data[i++]);
  }

  itr = array.Begin() + 3;
  *itr = 666;

  assert(*itr == 666);
  assert(array[3] == 666);

  array.Clear();
  assert(array.Size() == 0);
  assert(array.IsEmpty() == true);
}

template<class T>
class Increaser {
public:
  Increaser(const T& increase) : _increase(increase) { }

  void operator () (T& value) {
    value += _increase;
  }

private:
  T _increase;
};

void run_demo() {
  Array<Array<int> > array;
  int counter = 0;

  array.Resize(10);
  for(Array<Array<int> >::Iterator i = array.Begin(); i != array.End(); ++i) {
    i->Resize(10, counter++);
    std::for_each(i->Begin(), i->End(), Increaser<int>(11));
  }
  
  for(size_t i = 0; i < array.Size(); i++) {
    for(size_t j = 0; j < array[i].Size(); j++) {
      printf("%d ", array[i][j]);
    }
    printf("\n");
  }
}

int main() {
  run_tests();
  run_demo();
  return EXIT_SUCCESS;
}

