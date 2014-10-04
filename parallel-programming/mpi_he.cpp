#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>

#include <mpi.h>

double CalculateValue(double sigma, double left_value, double down_value, double left_down_value) {
  return left_down_value + (1.0 - sigma) / (1.0 + sigma) * down_value
    + (-1.0 + sigma) / (1.0 + sigma) * left_value;
}

int Next(int rank, int size) {
  return (rank + 1) % size;
}

int Prev(int rank, int size) {
  return (rank == 0) ? (size - 1) : (rank - 1);
}

int main (int argc, char** argv) {
  assert(argc == 2);
  int coordinate_steps = atoi(argv[1]);

  double length = 1.0;
  double total_time = 0.6;

  double sigma = 0.3;
  double lambda = 1.0;

  double boundary_value = 0.0;

  double coordinate_step = length / coordinate_steps;
  double time_step = sigma * coordinate_step / lambda;

  int time_steps = total_time / time_step;

  int rv = MPI_Init(&argc, &argv);
  assert(rv == MPI_SUCCESS);

  int size;
  rv = MPI_Comm_size(MPI_COMM_WORLD, &size);
  assert(rv == MPI_SUCCESS);

  int rank;
  rv = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  assert(rv == MPI_SUCCESS);

  MPI_Request request;
  MPI_Status status;

  std::vector<double> current_values(coordinate_steps, 0.0);
  std::vector<double> previous_values(coordinate_steps, 0.0);
  double start_time = MPI_Wtime();

  if (rank == 0) {
    for (int s = 0; s <= coordinate_steps; s++) {
      double coordinate = static_cast<double>(s) / coordinate_steps * length;
      if (coordinate >= 0.1 && coordinate <= 0.2) {
        current_values[s] = 1.0;
      }
    }
  }

  if (rank == 0) {
    for (int s = 0; s <= coordinate_steps; s++) {
      rv = MPI_Isend(&current_values[s], 1, MPI_DOUBLE, Next(rank, size), s, MPI_COMM_WORLD, &request);
      assert(rv == MPI_SUCCESS);
      rv = MPI_Request_free(&request);
      assert(rv == MPI_SUCCESS);
    }
  }

  for (int i = (rank == 0) ? 1 : 0; i * size + rank <= time_steps; i++) {
    for (int s = 0; s <= coordinate_steps; s++) {
      rv = MPI_Recv(&previous_values[s], 1, MPI_DOUBLE, Prev(rank, size), s, MPI_COMM_WORLD, &status);
      assert(rv == MPI_SUCCESS);

      double left_value = (s == 0) ? boundary_value : current_values[s - 1];
      double left_down_value = (s == 0) ? boundary_value : previous_values[s - 1];
      double down_value = previous_values[s];

      current_values[s] = CalculateValue(sigma, left_value, down_value, left_down_value);

      int receiver = (i * size + rank == time_steps) ? 0 : Next(rank, size);
      rv = MPI_Isend(&current_values[s], 1, MPI_DOUBLE, receiver, s, MPI_COMM_WORLD, &request);
      assert(rv == MPI_SUCCESS);
      rv = MPI_Request_free(&request);
      assert(rv == MPI_SUCCESS);
    }
  }

  if (rank == 0) {
    for (int s = 0; s <= coordinate_steps; s++) {
      int sender = time_steps % size;
      rv = MPI_Recv(&previous_values[s], 1, MPI_DOUBLE, sender, s, MPI_COMM_WORLD, &status);
      assert(rv == MPI_SUCCESS);
    }
  }

  if (rank == 0 && coordinate_steps <= 100) {
    for (int s = 0; s <= coordinate_steps; s++) {
      double coordinate = static_cast<double>(s) / coordinate_steps * length;
      printf("%lf %lf\n", coordinate, previous_values[s]);
    }
  }

  if (rank == 0) {
    double current_time = MPI_Wtime();
    printf("%d %lf\n", size, current_time - start_time);
  }

  rv = MPI_Finalize();
  assert(rv == MPI_SUCCESS);

  return EXIT_SUCCESS;
}
