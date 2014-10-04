#include <cassert>
#include <cstdlib>

class RC {
public:
  RC() : _counter(0) { }
  ~RC() { }

  void Up() {
    assert(_counter >= 0);
    _counter++;
  }

  void Down() {
    assert(_counter >= 1);
    _counter--;
  }

  void Set(int value) {
    assert(_counter >= 0);
    _counter = value;
  }

  int Get() const {
    assert(_counter >= 0);
    return _counter;
  }

private:
  int _counter;
};

template<class T>
class SP {
public:
  SP() {
    _data = new Data;
    assert(_data != NULL);
    _data->rc.Up();
  }

  SP(const SP& other) {
    _data = other._data;
    assert(_data != NULL);
    _data->rc.Up();
  }

  SP(const T& object) {
    _data = new Data;
    assert(_data != NULL);
    _data->pointer = new T(object);
    assert(_data->pointer != NULL);
    _data->rc.Up();
  }

  ~SP() {
    assert(_data != NULL);
    _dereference();
  }

  SP& operator = (const SP& other) {
    assert(_data != NULL);

    if(&other == this) {
      return *this;
    }
    //if(other._data == _data) {
    //  return *this;
    //}

    _dereference();

    _data = other._data;
    assert(_data != NULL);
    _data->rc.Up();

    //other._data = new Data;
    //assert(other._data != NULL);
    //other._data->rc.Up();

    return *this;
  }

  bool destroy_instance() {
    assert(_data != NULL);

    if(!is_instance()) {
      return false;
    }

    assert(_data->pointer != NULL);
    delete _data->pointer;
    _data->pointer = NULL;

    return true;
  }

  bool new_instance() {
    assert(_data != NULL);

    bool was_instance = is_instance();

    if(was_instance) {
      assert(_data->pointer != NULL);
      delete _data->pointer;
      _data->pointer = NULL;
    }

    _data->pointer = new T;
    assert(_data->pointer != NULL);

    return was_instance;
  }

  bool is_instance() const {
    assert(_data != NULL);
    return _data->pointer != NULL;
  }

  T& operator * () {
    assert(_data != NULL);
    if(!is_instance()) {
      throw "bad pointer";
    }
    return *_data->pointer;
  }

  T* operator -> () {
    assert(_data != NULL);
    if(!is_instance()) {
      throw "bad pointer";
    }
    return _data->pointer;
  }

private:
  struct Data {
    Data() : pointer(NULL) { }

    T* pointer;
    RC rc;
  };

  void _dereference() {
    assert(_data != NULL);

    _data->rc.Down();
    if(_data->rc.Get() == 0) {
      if(_data->pointer != NULL) {
        delete _data->pointer;
        _data->pointer = NULL;
      }
      delete _data;
      _data = NULL;
    }
  }

  Data* _data;
};

#include <iostream>
using namespace std;
#include <descriptor_main.h>
