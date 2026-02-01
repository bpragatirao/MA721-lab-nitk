#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    int rank, size;
    const int N = 600;  // Size of array
    int array[N];
    int local_max, global_max;
    int local_n;  // Elements per process
    double start_time, end_time;

    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 6) {
        if (rank == 0)
            printf("Please run with 6 processes \n");
        MPI_Finalize();
        return 0;
    }

    local_n = N / size;

    int local_array[local_n];

    // Initialize data only on root process
    if (rank == 0) {
        for (int i = 0; i < N; i++)
            array[i] = rand() % 1000;  // random numbers between 0 to 999
    }

    // Start timing after initialization
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    // Scatter the data from root to all processes
    MPI_Scatter(array, local_n, MPI_INT,
                local_array, local_n, MPI_INT,
                0, MPI_COMM_WORLD);

    // Compute local maximum
    local_max = local_array[0];
    for (int i = 1; i < local_n; i++) {
        if (local_array[i] > local_max)
            local_max = local_array[i];
    }

    // Reduce all local maxima to get the global maximum at root
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    // Stop timing
    end_time = MPI_Wtime();

    // Print result at root process
    if (rank == 0) {
        printf("Maximum value in the array = %d\n", global_max);
        printf("Time taken = %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
