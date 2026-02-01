#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TOTAL_STUDENTS 125
#define MAX_MARKS 100
#define BIN_SIZE 10  // Each bin covers a range of 10 marks

int main(int argc, char *argv[]) {
    int rank, size;
    int *marks = NULL;
    int local_n;
    double start_time, end_time;

    const int NUM_BINS = (MAX_MARKS / BIN_SIZE) + 1;  // 0 to 9, 10 to 19, ..., 100

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Determine local number of students per process
    local_n = TOTAL_STUDENTS / size;
    if (rank == 0 && TOTAL_STUDENTS % size != 0)
        printf("Warning: total students not perfectly divisible by process count\n");

    // Root generates random marks for all students
    if (rank == 0) {
        marks = (int *)malloc(TOTAL_STUDENTS * sizeof(int));
        srand(42);
        for (int i = 0; i < TOTAL_STUDENTS; i++)
            marks[i] = rand() % (MAX_MARKS + 1);  // marks between 0 to 100
    }

    int *local_marks = (int *)malloc(local_n * sizeof(int));

    // Synchronize before timing
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    // Scatter marks among all processes
    MPI_Scatter(marks, local_n, MPI_INT, local_marks, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process computes its local histogram
    int local_hist[NUM_BINS];
    for (int i = 0; i < NUM_BINS; i++)
        local_hist[i] = 0;

    for (int i = 0; i < local_n; i++) {
        int bin = local_marks[i] / BIN_SIZE;
        if (bin >= NUM_BINS)
            bin = NUM_BINS - 1;
        local_hist[bin]++;
    }

    // Combine all local histograms into the global histogram
    int global_hist[NUM_BINS];
    MPI_Reduce(local_hist, global_hist, NUM_BINS, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Stop timing
    end_time = MPI_Wtime();

    // Root prints the final histogram
    if (rank == 0) {
        printf("-------------------------------------------------\n");
        printf("MPI Histogram of Marks (Total Students: %d)\n", TOTAL_STUDENTS);
        printf("Number of processes: %d\n", size);
        printf("-------------------------------------------------\n");
        for (int i = 0; i < NUM_BINS; i++) {
            int lower = i * BIN_SIZE;
            int upper = (i == NUM_BINS - 1) ? MAX_MARKS : (lower + BIN_SIZE - 1);
            printf("Marks %3d - %3d : %3d students\n", lower, upper, global_hist[i]);
        }
        printf("-------------------------------------------------\n");
        printf("Parallel Execution Time: %f seconds\n", end_time - start_time);
        printf("-------------------------------------------------\n");
        free(marks);
    }

    free(local_marks);
    MPI_Finalize();
    return 0;
}
