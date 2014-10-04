#include <cassert>

#include <algorithm>
#include <iterator>

template<class T>
class Array {
public:
  typedef T value_type;

  class Iterator : public std::iterator<std::random_access_iterator_tag, value_type> {
  public:
    Iterator() : _element(NULL) { }
    Iterator(value_type* element) : _element(element) { }
    Iterator(const Iterator& iterator) : _element(iterator._element) { }
    ~Iterator() { }

    Iterator& operator ++ () {
      assert(_element != NULL);
      _element += 1;
      return *this;
    }

    Iterator operator ++ (int) {
      assert(_element != NULL);
      Iterator result(*this);
      _element += 1;
      return result;
    }

    Iterator operator += (size_t shift) {
      assert(_element != NULL);
      _element += shift;
      return *this;
    }

    Iterator operator + (size_t shift) {
      assert(_element != NULL);
      Iterator result(*this);
      result += shift;
      return result;
    }

    bool operator == (const Iterator& iterator) const {
      return _element == iterator._element;
    }

    bool operator != (const Iterator& iterator) const {
      return _element != iterator._element;
    }

    bool operator < (const Iterator& iterator) const {
      return _element < iterator._element;
    }

    value_type& operator * () {
      assert(_element != NULL);
      return *_element;
    }

    value_type* operator -> () {
      assert(_element != NULL);
      return _element;
    }

  private:
    value_type* _element;
  };

  Array() : _size(0), _capacity(0), _data(NULL) { }
  ~Array() {
    if(_data != NULL) {
      delete[] _data;
    }
  }

  value_type& operator [] (size_t index) {
    assert(index < _size);
    return _data[index];
  }

  void PushBack(const value_type& value) {
    _EnsureCapacity(_size + 1);
    _data[_size] = value;
    _size += 1;
  }

  void Resize(size_t size, const value_type& value) {
    _EnsureCapacity(size);
    for(size_t i = _size; i <= size; i++) {
      _data[i] = value;
    }
    _size = size;
  }
  void Resize(size_t size) {
    Resize(size, value_type());
  }

  void Reserve(size_t capacity) {
    _EnsureCapacity(capacity);
  }

  void Clear() {
    _size = 0;
  }

  size_t Size() const {
    return _size;
  }
  bool IsEmpty() const {
    return _size == 0;
  }

  Iterator Begin() {
    return Iterator(_data);
  }
  Iterator End() {
    return Iterator(_data + _size);
  }

private:
  void _EnsureCapacity(size_t capacity) {
    if(_capacity < capacity) {
      size_t new_capacity = std::max<size_t>(_capacity, 1);
      while(new_capacity < capacity) {
        new_capacity *= 2;
      }
      value_type* new_data = new value_type[new_capacity];
      std::copy(_data, _data + _size, new_data);
      if(_data != NULL) {
        delete[] _data;
      }
      _data = new_data;
      _capacity = new_capacity;
    }
  }

  value_type* _data;
  size_t _size;
  size_t _capacity;
};

