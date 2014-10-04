#include <cassert>
#include <cstring>

#include <iostream>
#include <string>

#include "markov.h"

void test0() {
  M<char> m('\0', 2);

  assert( m.add_rule("AAA", "B", true) );
  assert( m.add_rule("", "A", false) );
  assert( !m.add_rule("A", "C", true) );
  assert( m.run(100) == 4);
  assert( strcmp(m.get_state(), "B") == 0);

  M<int> m2(0, 10);
  m2.set_state(5, 0);
  assert( m2.add_rule(false, 5, 0, 4, 4, 0) );
  assert( m2.add_rule(false, 4, 0, 3, 3, 0) );
  assert( m2.add_rule(false, 3, 0, 2, 2, 0) );
  assert( m2.add_rule(false, 2, 0, 1, 1, 0) );
  assert( m2.run(14) == -1 );
  assert( m2.run(14) == 1 );
  assert( m2.run() == 0 );
  assert( m2.get_state()[15] == 1 );
}

void test1() {
  M<char> m('\0', 0);
  const char* s = m.get_state();
  assert(s != NULL);
  assert(s[0] == '\0');
 
  m.set_state("test");
  s = m.get_state();
  assert(s[0] == 't' && s[1] == 'e' && s[2] == 's' && s[3] == 't' && s[4] == '\0');

  m.set_state("qwe");
  s = m.get_state();
  assert(s[0] == 'q' && s[1] == 'w' && s[2] == 'e' && s[3] == '\0');

  assert(m.add_rule("asd", "", true) == false);
  assert(m.run() == 0);
  assert(m.run(100) == 0);
}

void test2() {
  M<char> m('x', 0);
  const char* s = m.get_state();
  assert(s != NULL);
  assert(s[0] == 'x');
 
  m.set_state("testx");
  s = m.get_state();
  assert(s[0] == 't' && s[1] == 'e' && s[2] == 's' && s[3] == 't' && s[4] == 'x');

  m.set_state("qwex");
  s = m.get_state();
  assert(s[0] == 'q' && s[1] == 'w' && s[2] == 'e' && s[3] == 'x');

  assert(m.add_rule("asdx", "x", true) == false);
  assert(m.run() == 0);
  assert(m.run(100) == 0);
}

void test3() {
  M<char> m('x', 2);
  const char* s = m.get_state();
  assert(s != NULL);
  assert(s[0] == 'x');

  assert(m.add_rule("bbbx", "ax", true) == true);
  assert(m.add_rule("x", "bx", false) == true); 
  assert(m.add_rule("x", "bx", false) == false);

  assert(m.run() == 4);
  s = m.get_state();
  assert(s[0] == 'a' && s[1] == 'x');

  assert(m.run() == 4);
  s = m.get_state();
  assert(s[0] == 'a' && s[1] == 'a' && s[2] == 'x');

  m.set_state("bbbx");
  assert(m.run() == 1);
  assert(m.run() == 4);
  s = m.get_state();
  assert(s[0] == 'a' && s[1] == 'a' && s[2] == 'x');
}

void test4() {
  M<int> m(-1, 5);
  assert(m.get_state()[0] == -1);
  assert(m.add_rule(false, -1, -1) == true);
  assert(m.run(123) == -1);

  M<int> n(-1, 5);
  assert(n.add_rule(false, 1, -1, -1) == true);
  n.set_state(1, 1, -1);
  assert(n.run(100) == 2);
  
  assert(n.add_rule(true, -1, 1, 1, 1, -1) == true);
  assert(n.run(100) == 1);
  assert(n.run(123) == 4);
}

int main(){
  test0();
  test1();
  test2();
  test3();
  test4();
  return 0;
}
