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

  assert(size > 1);

  if (rank == 0) {
    int array_size = 1000;

    int* array = new int[array_size];
    assert(array != NULL);

    // Filling the array.
    for (int i = 0; i < array_size; i++) {
      array[i] = i;
    }

    int sent_size = 0;
    for (int i = 1; i < size; i++) {
      int part_size = array_size / (size - 1);
      if (i < array_size % (size - 1) + 1) {
        part_size++;
      }

      rv = MPI_Send(&part_size, 1 /* count */, MPI_INT,
        i /* destination */, 0 /* tag */, MPI_COMM_WORLD);
      assert(rv == MPI_SUCCESS);

      rv = MPI_Send(array + sent_size, part_size, MPI_INT,
        i /* destination */, 1 /* tag */, MPI_COMM_WORLD);
      assert(rv == MPI_SUCCESS);

      sent_size += part_size;
    }

    printf("P%d: sent out the parts of the array.\n", rank);

    int my_sum = 0;
    for (int i = 0; i < array_size; i++) {
      my_sum += array[i];
    }

    printf("P%d: calculated the sum of the whole array: %d.\n", rank, my_sum);

    int sum = 0;
    for (int i = 1; i < size; i++) {
      int part_sum = 0;
      rv = MPI_Recv(&part_sum, 1 /* count */, MPI_INT,
        i /* source */, 2 /* tag */, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      assert(rv == MPI_SUCCESS);
      sum += part_sum;
    }

    printf("P%d: received the sum of the array: %d.\n", rank, sum);
  } else {
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

/*
scp -P 22805 <имя файла> 807303@openmp.mipt.ru: /directory/<имя файла>

переименование:
mv <old_name> <new_name.c> 


Process0 заполняет:
a[N + 1], N = 1000
a[i] = i

n процессов

P0: разбивает a[] на n процессов и отсылает каждому свою часть.
Pj: получает часть массива, считает сумму, отсылает назад.
P0: получает, складывает, выводит. (сам он может не считать)
    + сам считает всю сумму

   N1                N2
Send -> N2       Send -> N1
Recv <- N2       Recv <- N2

1) смена мест операторов
2) Isend, Irecv
*/
