#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <vector>

#include <omp.h>

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

  std::vector<Value> As(M + 1);
  std::vector<Value> Bs(M + 1);
  std::vector<Value> Cs(M + 1);
  std::vector<Value> Fs(M + 1);

  As[0] = Value(0.0, 0.0);
  for (int m = 1; m <= M; m++) {
    As[m] = -A[m] * A[m - 1] / B[m - 1];
  }

  Bs[0] = B[0] - C[0] * A[1] / B[1];
  for (int m = 1; m <= M - 1; m++) {
    Bs[m] = B[m] - A[m] * C[m - 1] / B[m - 1] - C[m] * A[m + 1] / B[m + 1];
  }
  Bs[M] = B[M] - A[M] * C[M - 1] / B[M - 1];

  for (int m = 0; m <= M - 1; m++) {
    Cs[m] = -C[m] * C[m + 1] / B[m + 1];
  }
  Cs[M] = Value(0.0, 0.0);

  std::vector<Value> prev_values(M + 1);
  std::vector<Value> curr_values(M + 1);

  InitializeValues(curr_values);

  std::vector<Value> P(M + 1);
  std::vector<Value> Q(M + 1);

  #pragma omp parallel num_threads(2)
  {
  int rank = omp_get_thread_num();
  for (int n = 0; n < N; n++) {
    #pragma omp barrier

    if (rank == 0) {
      prev_values.swap(curr_values);
    }

    #pragma omp barrier

    if (rank == 0) {
      F[0] = Value(0.0, 0.0);
      F[M] = Value(0.0, 0.0);
    }
    for (int m = 1; m <= M - 1; m++) {
      if (m % 2 == rank) {
        F[m].u = -prev_values[m].u / tau - f(prev_values[m]);
        F[m].v = -prev_values[m].v / tau - g(prev_values[m]);
      }
    }

    #pragma omp barrier

    Fs[0] = F[0] - C[0] / B[1] * F[1];
    for (int m = 1; m <= M - 1; m++) {
      if (m % 2 == rank) {
        Fs[m] = F[m] - A[m] / B[m - 1] * F[m - 1] - C[m] / B[m + 1] * F[m + 1];
      }
    }
    Fs[M] = F[M] - A[M] / B[M - 1] * F[M - 1];

    #pragma omp barrier

    if (rank == 0) {
      P[0] = -Cs[0] / Bs[0];
      Q[0] = Fs[0] / Bs[0];
    } else {
      P[1] = -Cs[1] / Bs[1];
      Q[1] = Fs[1] / Bs[1];
    }

    for (int m = 2; m <= M; m++) {
      if (m % 2 == rank) {
        P[m] = -Cs[m] / (As[m] * P[m - 2] + Bs[m]);
        Q[m] = (Fs[m] - As[m] * Q[m - 2]) / (As[m] * P[m - 2] + Bs[m]);
      }
    }

    if (M % 2 == rank) {
      curr_values[M] = Q[M];
    } else {
      curr_values[M - 1] = Q[M - 1];
    }

    for (int m = M - 2; m >= 0; m--) {
      if (m % 2 == rank) {
        curr_values[m] = P[m] * curr_values[m + 2] + Q[m];
      }
    }

    #pragma omp barrier
  }
  }  // pragma omp

  for (int m = 0; m <= M; m++) {
    double coord = X * m / M;
    printf("%lf %lf %lf\n", coord, curr_values[m].u, curr_values[m].v);
  }

  return EXIT_SUCCESS;
}
