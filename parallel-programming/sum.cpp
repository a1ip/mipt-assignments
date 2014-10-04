#include <assert.h>

#include <mpi.h>

int main(int argc, char** argv) {
  int rv = MPI_Init(&argc, &argv);
  assert(rv == MPI_SUCCESS);

  int size;
  rv = MPI_Comm_size(MPI_COMM_WORLD, &size);
  assert(rv == MPI_SUCCESS);

  int rank;
  rv = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  assert(rv == MPI_SUCCESS);

  assert(size == 2);

  if (rank == 0) {
    int array_size = 12376;
    int first_part_size = 5373;
    int second_part_size = array_size - first_part_size;

    assert(first_part_size < array_size);

    int* array = new int[array_size];
    assert(array != NULL);

    // Filling the array.
    for (int i = 0; i < array_size; i++) {
      array[i] = i + 1;
    }

    rv = MPI_Send(&second_part_size, 1 /* count */, MPI_INT,
      1 /* destination */, 0 /* tag */, MPI_COMM_WORLD);
    assert(rv == MPI_SUCCESS);

    printf("P%d: sent the size of the second part of the array: %d.\n",
      rank, second_part_size);

    rv = MPI_Send(array + first_part_size, second_part_size, MPI_INT,
      1 /* destination */, 1 /* tag */, MPI_COMM_WORLD);
    assert(rv == MPI_SUCCESS);

    printf("P%d: sent the second part of the array.\n",
      rank, second_part_size);

    int first_part_sum = 0;
    for (int i = 0; i < first_part_size; i++) {
      first_part_sum += array[i];
    }

    printf("P%d: calculated the sum of the first part the array: %d.\n",
      rank, first_part_sum);

    int second_part_sum = 0;

    rv = MPI_Recv(&second_part_sum, 1 /* count */, MPI_INT,
      1 /* source */, 2 /* tag */, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    assert(rv == MPI_SUCCESS);

    printf("P%d: received the sum of the second part the array: %d.\n",
      rank, second_part_sum);

    int sum = first_part_sum + second_part_sum;

    printf("P%d: the total sum of the array: %d.\n",
      rank, sum);

  } else if (rank == 1) {
    int array_size = 0;

    rv = MPI_Recv(&array_size, 1 /* count */, MPI_INT,
      0 /* source */, 0 /* tag */, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    assert(rv == MPI_SUCCESS);

    printf("P%d: received the size: %d.\n",
      rank, array_size);

    int* array = new int[array_size];
    assert(array != NULL);

    rv = MPI_Recv(array, array_size, MPI_INT,
      0 /* source */, 1 /* tag */, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    assert(rv == MPI_SUCCESS);

    printf("P%d: received the array of %d elements.\n", rank, array_size);

    int sum = 0;
    for (int i = 0; i < array_size; i++) {
      sum += array[i];
    }

    printf("P%d: calculated the sum of the array: %d.\n", rank, sum);

    rv = MPI_Send(&sum, 1 /* count */, MPI_INT,
      0 /* destination */, 2 /* tag */, MPI_COMM_WORLD);
    assert(rv == MPI_SUCCESS);

    printf("P%d: sent the sum of the array back.\n", rank);
  }

  rv = MPI_Finalize();
  assert(rv == MPI_SUCCESS);

  return 0;
}
