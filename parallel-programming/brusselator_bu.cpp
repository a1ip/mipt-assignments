#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <vector>

//#include <mpi.h>

const double T = 100.0;
const double X = 200.0;

const double d = 1.0;
const double D = 16.0;
const double a = 1.8;
const double b = 1.0;

const double tau = 0.01;
const double h = 0.01;

const int N = T / tau;
const int M = X / h;

struct Value {
  Value() : u(0.0), v(0.0) { }
  Value(double u, double v) : u(u), v(v) { }

  double u, v;
};

Value operator - (const Value& v) {
  return Value(-v.u, -v.v);
}

Value operator + (const Value& v1, const Value& v2) {
  return Value(v1.u + v2.u, v1.v + v2.v);
}

Value operator - (const Value& v1, const Value& v2) {
  return Value(v1.u - v2.u, v1.v - v2.v);
}

Value operator * (const Value& v1, const Value& v2) {
  return Value(v1.u * v2.u, v1.v * v2.v);
}

Value operator / (const Value& v1, const Value& v2) {
  return Value(v1.u / v2.u, v1.v / v2.v);
}

double Random() {
  return static_cast<double>(rand()) / RAND_MAX;
}

void InitializeValues(std::vector<Value>& values) {
  values.resize(M);
  for (int i = 0; i <= M; i++) {
    values[i].u = b * (0.5 + Random());
    values[i].v = a / b * (0.5 + Random());
  }
}

double f(double u, double v) {
  return b + u * u * v - u - a * u;
}

double g(double u, double v) {
  return a * u - u * u * v;
}

Value F(std::vector<Value>& values, int m) {
  Value r;
  r.u = -values[m].u / tau - f(values[m].u, values[m].v);
  r.v = -values[m].v / tau - g(values[m].u, values[m].v);
  return r;
}

int main(int argc, char** argv) {
  Value A(d / h / h, D / h / h);
  Value B(-2.0 * d / h / h - 1.0 / tau, -2.0 * D / h / h - 1.0 / tau);
  Value C = A;

  Value B0(1.0, 1.0);
  Value C0(-1.0, -1.0);
  Value F0(0.0, 0.0);

  Value AM = B0; 
  Value BM = C0;
  Value FM = F0;

  std::vector<Value> prev_values(M + 1);
  std::vector<Value> curr_values(M + 1);

  InitializeValues(curr_values);

  std::vector<Value> P(M + 1);
  std::vector<Value> Q(M + 1);

  for (int n = 0; n < N; n++) {
    prev_values.swap(curr_values);

    P[0] = -C0 / B0;
    Q[0] = F0 / B0;

    for (int m = 1; m <= M - 1; m++) {
      P[m] = -C / (A * P[m - 1] + B);
      Q[m] = (F(prev_values, m) - A * Q[m - 1]) / (A * P[m - 1] + B);
    }

    Q[M] = (FM - AM * Q[M - 1]) / (AM * P[M - 1] + BM);

    curr_values[M] = Q[M];
    for (int m = M - 1; m >= 0; m--) {
      curr_values[m] = P[m] * curr_values[m + 1] + Q[m];
    }
  }

  for (int m = 0; m < M; m++) {
    double coord = X * m / M;
    printf("%lf %lf %lf\n", coord, curr_values[m].u, curr_values[m].v);
  }

  return EXIT_SUCCESS;
}
