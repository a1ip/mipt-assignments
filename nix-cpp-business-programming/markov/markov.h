#include <cassert>
#include <cstdarg>

#include <algorithm>
#include <vector>

template<class T>
class M {
public:
  M(T end_marker, int rules_limit = 100) : _end_marker(end_marker), _rules_limit(rules_limit) {
    _state.push_back(_end_marker);
  }

  ~M() { }

  bool add_rule(const T* from, const T* to, bool is_termimating) {
    if(_rules.size() == _rules_limit) {
      return false;
    }
    Rule rule;
    translate_string(from, &rule.from);
    translate_string(to, &rule.to);
    rule.is_termimating = is_termimating;
    _rules.push_back(rule);
    return true;
  }

  bool add_rule(bool is_terminating, ...) {
    if(_rules.size() == _rules_limit) {
      return false;
    }

    Rule rule;
    rule.is_termimating = is_terminating;
    
    va_list(arguments);
    va_start(arguments, is_terminating);

    T element = va_arg(arguments, T);
    while(element != _end_marker) {
      rule.from.push_back(element);
      element = va_arg(arguments, T);
    }
    element = va_arg(arguments, T);
    while(element != _end_marker) {
      rule.to.push_back(element);
      element = va_arg(arguments, T);
    }

    _rules.push_back(rule);
    return true;
  }

  void set_state(const T* state) {
    translate_string(state, &_state);
    _state.push_back(_end_marker);
  }

  void set_state(T element, ...) {
    _state.clear();

    va_list arguments;
    va_start(arguments, element);
    
    while(element != _end_marker) {
      _state.push_back(element);
      element = va_arg(arguments, T);
    }

    va_end(arguments);

    _state.push_back(_end_marker);
  }

  const T* get_state() const {
    return &_state[0];
  }

  int run(int steps_limit = 1000000) {
    for(int step = 1; step <= steps_limit; step++) {
      int rv = make_step();
      if(rv == 1) {
        return step;
      } else if(rv == -1) {
        return step - 1;
      }
    }
    return -1;
  }

private:
  M();
  M(const M& other);
  M& operator = (const M& other);

  struct Rule {
    std::vector<T> from;
    std::vector<T> to;
    bool is_termimating;
  };

  void translate_string(const T* input, std::vector<T>* output) {
    size_t input_size = 0;
    while(input[input_size] != _end_marker) {
      input_size++;
    }
    output->assign(input, input + input_size);
  }

  // Returns '1' if the step was made and the applied rule was terminating.
  // Returns '0' if the step was made and the applied rule was not terminating.
  // Returns '-1' if no rules were found to apply.
  int make_step() {
    for(size_t rule = 0; rule < _rules.size(); rule++) {
      typename std::vector<T>::iterator entry = std::search(_state.begin(), _state.end(),
        _rules[rule].from.begin(), _rules[rule].from.end());
      if(entry != _state.end()) {
        typename std::vector<T>::iterator i = _state.erase(entry, entry + _rules[rule].from.size());
        _state.insert(i, _rules[rule].to.begin(), _rules[rule].to.end());
        return _rules[rule].is_termimating ? 1 : 0;
      }
    }
    return -1;
  }

  T _end_marker;
  int _rules_limit;

  std::vector<T> _state; // With '_end_marker' in the end.
  std::vector<Rule> _rules;
};

#include <iostream> // WTF?!?!?!
using namespace std; // WTF?!?!?!

#include <markov_main.h>
