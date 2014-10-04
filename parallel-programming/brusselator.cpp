#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <vector>

const double T = 1000.0;
const double X = 200.0;

const double d = 1.0;
const double D = 16.0;
const double a = 1.8;
const double b = 1.0;

const double tau = 0.1;
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

double f(const Value& val) {
  return b + val.u * val.u * val.v - val.u - a * val.u;
}

double g(const Value& val) {
  return a * val.u - val.u * val.u * val.v;
}

Value F(const Value& value) {
  Value r;
  r.u = -value.u / tau - f(value);
  r.v = -value.v / tau - g(value);
  return r;
}

int main(int argc, char** argv) {
  std::vector<Value> A(M + 1);
  std::vector<Value> B(M + 1);
  std::vector<Value> C(M + 1);
  std::vector<Value> F(M + 1);

  A[0] = Value(0.0, 0.0);  // Not used.
  for (int m = 1; m <= M - 1; m++) {
    A[m] = Value(d / h / h, D / h / h);
  }
  A[M] = Value(1.0, 1.0);

  B[0] = Value(1.0, 1.0);
  for (int m = 1; m <= M - 1; m++) {
    B[m] = Value(-2.0 * d / h / h - 1.0 / tau, -2.0 * D / h / h - 1.0 / tau);
  }
  B[M] = Value(-1.0, -1.0);

  C[0] = Value(-1.0, -1.0);
  for (int m = 1; m <= M - 1; m++) {
    C[m] = Value(d / h / h, D / h / h);
  }
  C[M] = Value(0.0, 0.0);  // Not used.

  std::vector<Value> prev_values(M + 1);
  std::vector<Value> curr_values(M + 1);

  InitializeValues(curr_values);

  std::vector<Value> P(M + 1);
  std::vector<Value> Q(M + 1);

  for (int n = 0; n < N; n++) {
    prev_values.swap(curr_values);

    F[0] = Value(0.0, 0.0);
    for (int m = 1; m <= M - 1; m++) {
      F[m].u = -prev_values[m].u / tau - f(prev_values[m]);
      F[m].v = -prev_values[m].v / tau - g(prev_values[m]);
    }
    F[M] = Value(0.0, 0.0);

    P[0] = -C[0] / B[0];
    Q[0] = F[0] / B[0];

    for (int m = 1; m <= M - 1; m++) {
      P[m] = -C[m] / (A[m] * P[m - 1] + B[m]);
      Q[m] = (F[m] - A[m] * Q[m - 1]) / (A[m] * P[m - 1] + B[m]);
    }

    Q[M] = (F[M] - A[M] * Q[M - 1]) / (A[M] * P[M - 1] + B[M]);

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
