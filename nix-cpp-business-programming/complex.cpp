#include <cassert>
#include <cmath>

#include <iostream>

class C {
 public:
  C() : re(0.0), im(0.0) { }
  C(double re) : re(re), im(0.0) { }
  C(double re, double im) : re(re), im(im) { }

  double abs() const {
    return sqrt(re * re + im * im);
  }

  C operator - () {
    return C(-re, -im);
  }

  C& operator += (const C& other) {
    re += other.re;
    im += other.im;
    return *this;
  }

  C& operator -= (const C& other) {
    re -= other.re;
    im -= other.im;
    return *this;
  }

  C& operator *= (const C& other) {
    double old_re = re;
    double old_im = im;
    re = old_re * other.re - old_im * other.im;
    im = old_re * other.im + old_im * other.re;
    return *this;
  }

  C& operator /= (const C& other) {
    assert(other.re != 0.0 || other.im != 0.0);
    double abs2 = other.re * other.re + other.im * other.im;
    double old_re = re;
    double old_im = im;
    re = (old_re * other.re + old_im * other.im) / abs2;
    im = (old_im * other.re - old_re * other.im) / abs2;
    return *this;
  }

  C& operator += (double other) {
    re += other;
    return *this;
  }

  C& operator -= (double other) {
    re -= other;
    return *this;
  }

  C& operator *= (double other) {
    re *= other;
    im *= other;
    return *this;
  }

  C& operator /= (double other) {
    assert(other != 0.0);
    re /= other;
    im /= other;
    return *this;
  }

 public:
  double re, im;
};

C operator + (const C& first, const C& second) {
  C result = first;
  result += second;
  return result;
}

C operator - (const C& first, const C& second) {
  C result = first;
  result -= second;
  return result;
}

C operator * (const C& first, const C& second) {
  C result = first;
  result *= second;
  return result;
}

C operator / (const C& first, const C& second) {
  C result = first;
  result /= second;
  return result;
}


C operator + (const C& complex, double scalar) {
  C result = complex;
  result += scalar;
  return result;
}

C operator - (const C& complex, double scalar) {
  C result = complex;
  result -= scalar;
  return result;
}

C operator * (const C& complex, double scalar) {
  C result = complex;
  result *= scalar;
  return result;
}

C operator / (const C& complex, double scalar) {
  C result = complex;
  result /= scalar;
  return result;
}


C operator + (double scalar, const C& complex) {
  C result = C(scalar);
  result += complex;
  return result;
}

C operator - (double scalar, const C& complex) {
  C result = C(scalar);
  result -= complex;
  return result;
}

C operator * (double scalar, const C& complex) {
  C result = C(scalar);
  result *= complex;
  return result;
}

C operator / (double scalar, const C& complex) {
  C result = C(scalar);
  result /= complex;
  return result;
}


bool operator == (const C& first, const C& second) {
  return first.re == second.re && first.im == second.im; 
}

bool operator != (const C& first, const C& second) {
  return !(first == second); 
}


std::ostream& operator << (std::ostream& output, const C& complex) {
  output << complex.re << " " << complex.im;
  return output;
}

std::istream& operator >> (std::istream& input, C& complex) {
  input >> complex.re >> complex.im;
  return input;
}

using namespace std; // For 'complex_main.h'.

//#include <complex_main.h>

int main() {
  C a(0.0, 0.0), b(-2.0, 0.0), c(0.0, 3.0), d(-4.0, 3.0);

  //std::cin >> a >> b;
  //std::cout << a << ", " << b << ", " << c << ", " << d << std::endl;

  assert(a.abs() == 0.0);
  assert(b.abs() == 2.0);
  assert(c.abs() == 3.0);
  assert(d.abs() == 5.0);

  a = d;
  b = -d;
  assert(a.re == d.re && a.im == d.im);
  assert(b.re == -d.re && b.im == -d.im);

  a = C(1.0, -2.2323);
  c = C(-1.0, 0.2323);
  a += c;
  assert(a.re == 0.0 && a.im == -2.0);
  assert(c.re == -1.0 && c.im == 0.2323);

  a -= c;
  assert(a.re == 1.0 && a.im == -2.2323);
  assert(c.re == -1.0 && c.im == 0.2323);

  a = C(1.0, 3.0);
  b = C(-10.0, 5.0);
  a *= b;
  assert(a.re == -25.0 && a.im == -25.0);
  assert(b.re = -10.0 && b.im == 5.0);

  a = C(2.0, 5.0);
  b = C(-2.0, 1.0);
  a /= b;
  assert(a.re == 0.2 && a.im == -2.4);
  assert(b.re == -2.0 && b.im == 1.0);

  a = C(2.0, 5.0);
  b = C(-2.0, 1.0);
 
  c = a + b;
  assert(c.re == 0.0 && c.im == 6.0);

  c = a - b;
  assert(c.re == 4.0 && c.im == 4.0);

  c = a * b;
  assert(c.re == -9.0 && c.im == -8.0);

  c = a / b;
  assert(c.re == 0.2 && c.im == -2.4);

  a = C(3.0, -10.0);
  
  assert(a + 5.0 == 5.0 + a);
  assert(a - 5.0 == -5.0 + a);
  assert(a * 12.23 == 12.23 * a);
  assert(a / 12.23 == 1.0 / 12.23 * a);

  assert(a == 1.0 / (1.0 / a));

  assert(a * b == b * a);
  assert(a * (b + c) == a * b + a * c);
  assert(a * (-b * (c - d)) == -a * b * c + a * b * d);

  return 0;
}

