#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int N = 1000000;  // Default array size (modifiable)
    int *array = NULL;
    int local_n;
    int local_min, local_max;
    int global_min, global_max;
    double start_time, end_time, local_start, local_end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Allow optional command-line argument: array size
    if (argc > 1)
        N = atoi(argv[1]);

    local_n = N / size;
    int *local_array = (int *)malloc(local_n * sizeof(int));

    // Only the root process initializes the full array
    if (rank == 0) {
        array = (int *)malloc(N * sizeof(int));
        srand(42); // deterministic random seed
        for (int i = 0; i < N; i++)
            array[i] = rand() % 100000;  // random numbers 0 to 99999
    }

    // Synchronize before timing
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    // Scatter data to all processes
    MPI_Scatter(array, local_n, MPI_INT, local_array, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process computes local min and max
    local_min = local_array[0];
    local_max = local_array[0];
    for (int i = 1; i < local_n; i++) {
        if (local_array[i] < local_min) local_min = local_array[i];
        if (local_array[i] > local_max) local_max = local_array[i];
    }

    // Reduce local minima and maxima to global values at root
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    // Stop timing
    end_time = MPI_Wtime();

    // Root prints the result
    if (rank == 0) {
        printf("-----------------------------------------------------\n");
        printf("Array size (N): %d\n", N);
        printf("Number of processes: %d\n", size);
        printf("Global Minimum: %d\n", global_min);
        printf("Global Maximum: %d\n", global_max);
        printf("Parallel Execution Time: %f seconds\n", end_time - start_time);
        printf("-----------------------------------------------------\n");
        free(array);
    }

    free(local_array);
    MPI_Finalize();
    return 0;
}
