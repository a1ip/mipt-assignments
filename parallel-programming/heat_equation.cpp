#include <assert.h>
#include <stdlib.h>

#include <vector>

#include <mpi.h>

const double length = 1.0;
const double boundary_temperature = 0.0;
const double initial_temperature = 1.0;
const double coefficient = 1.0;
const double finish_time = 0.1;
const double coordinate_step = 0.1;
const double time_step = 0.001;

void make_time_step(const std::vector<double>& current_values,
    double left_value, double right_value, std::vector<double>& next_values) {
  assert(current_values.size() == next_values.size());

  for (size_t i = 0; i < current_values.size(); i++) {
    double left = (i == 0 ? left_value : current_values[i - 1]);
    double middle = current_values[i];
    double right = (i + 1 == current_values.size() ? right_value : current_values[i + 1]);
    next_values[i] = middle + time_step * coefficient * (left - 2.0 * middle + right) /
      (coordinate_step * coordinate_step);
  }
}

void exchange_values(int rank, int size, double next_left, double next_right,
    double* current_left, double* current_right) {
  if (rank % 2 == 0) {
    if (rank != 0) {
      int rv = MPI_Send(&next_left, 1 /* count */, MPI_DOUBLE,
        rank - 1 /* destination */, 0 /* tag */, MPI_COMM_WORLD);
      assert(rv == MPI_SUCCESS);
    }
    if (rank + 1 != size) {
      int rv = MPI_Send(&next_right, 1 /* count */, MPI_DOUBLE,
        rank + 1 /* destination */, 1 /* tag */, MPI_COMM_WORLD);
      assert(rv == MPI_SUCCESS);
    }

    if (rank != 0) {
      int rv = MPI_Recv(current_left, 1 /* count */, MPI_DOUBLE,
        rank - 1 /* source */, 2 /* tag */, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      assert(rv == MPI_SUCCESS);
    }
    if (rank + 1 != size) {
      int rv = MPI_Recv(current_right, 1 /* count */, MPI_DOUBLE,
        rank + 1 /* source */, 3 /* tag */, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      assert(rv == MPI_SUCCESS);
    }
  } else {
    if (rank + 1 != size) {
      int rv = MPI_Recv(current_right, 1 /* count */, MPI_DOUBLE,
        rank + 1 /* source */, 0 /* tag */, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      assert(rv == MPI_SUCCESS);
    }
    if (rank != 0) {
      int rv = MPI_Recv(current_left, 1 /* count */, MPI_DOUBLE,
        rank - 1 /* source */, 1 /* tag */, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      assert(rv == MPI_SUCCESS);
    }

    if (rank + 1 != size) {
      int rv = MPI_Send(&next_right, 1 /* count */, MPI_DOUBLE,
        rank + 1 /* destination */, 2 /* tag */, MPI_COMM_WORLD);
      assert(rv == MPI_SUCCESS);
    }
    if (rank != 0) {
      int rv = MPI_Send(&next_left, 1 /* count */, MPI_DOUBLE,
        rank - 1 /* destination */, 3 /* tag */, MPI_COMM_WORLD);
      assert(rv == MPI_SUCCESS);
    }
  }
}

int main (int argc, char** argv) {
  int rv = MPI_Init(&argc, &argv);
  assert(rv == MPI_SUCCESS);

  int size;
  rv = MPI_Comm_size(MPI_COMM_WORLD, &size);
  assert(rv == MPI_SUCCESS);

  int rank;
  rv = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  assert(rv == MPI_SUCCESS);

  double start_time = MPI_Wtime();

  int points = static_cast<int>(length / coordinate_step) - 1;
  assert(points > 0);

  int workers = (size <= points ? size : points);
  if (rank >= points) {
    rv = MPI_Finalize();
    assert(rv == MPI_SUCCESS);
    return EXIT_SUCCESS;
  }

  int current_points = points / workers;
  if (rank < points % workers) {
    current_points += 1;
  }
  assert(current_points > 0);

  int first_point = rank * (points / workers) + (rank < points % workers ? rank : points % workers);

  std::vector<double> current_values(current_points, initial_temperature);
  std::vector<double> next_values(current_points, -1.0);

  double left_value = (rank == 0 ? boundary_temperature : initial_temperature);
  double right_value = (rank + 1 == workers ? boundary_temperature : initial_temperature);

  int time_steps = static_cast<int>(finish_time / time_step);

  for (int i = 0; i < time_steps; i++) {
    make_time_step(current_values, left_value, right_value, next_values);
    exchange_values(rank, workers, next_values.front(), next_values.back(), &left_value, &right_value);
    current_values.swap(next_values);
  }

  if (rank == 0) {
    std::vector<double> result(points, -1.0);

    std::copy(current_values.begin(), current_values.end(), result.begin());
    int offset = current_values.size();
    
    for (int i = 1; i < workers; i++) {
      int ith_size = 0;
      int rv = MPI_Recv(&ith_size, 1 /* count */, MPI_INT,
      i /* source */, i * 2 /* tag */, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      assert(rv == MPI_SUCCESS);

      rv = MPI_Recv(&result[offset], ith_size /* count */, MPI_DOUBLE,
      i /* source */, i * 2 + 1 /* tag */, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      assert(rv == MPI_SUCCESS);

      offset += ith_size;
    }

    printf("%lf ", boundary_temperature);
    for (size_t i = 0; i < result.size(); i++) {
      printf("%lf ", result[i]);
    }
    printf("%lf\n", boundary_temperature);
  } else {
    int rv = MPI_Send(&current_points, 1 /* count */, MPI_INT,
      0 /* destination */, rank * 2 /* tag */, MPI_COMM_WORLD);
    assert(rv == MPI_SUCCESS);

    rv = MPI_Send(&current_values[0], current_points, MPI_DOUBLE,
      0 /* destination */, rank * 2 + 1 /* tag */, MPI_COMM_WORLD);
    assert(rv == MPI_SUCCESS);
  }

  if (rank == 0) {
    double current_time = MPI_Wtime();
    printf("Elapsed time: %lf.\n", current_time - start_time);
  }

  rv = MPI_Finalize();
  assert(rv == MPI_SUCCESS);

  return EXIT_SUCCESS;
}
