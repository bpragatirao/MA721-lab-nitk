#include <stdio.h>
#include <math.h>
#include <mpi.h>

// Define the function to integrate: f(x) = x^2
double f(double x) {
    return x * x;
}

int main(int argc, char *argv[]) {
    int rank, size, n;
    double a = 0.0, b = 10.0;  // integration limits
    double h, local_a, local_b;
    double local_integral, total_integral;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Default number of intervals
    n = 1000000;
    if (argc > 1) {
        n = atoi(argv[1]); // read from command line
    }

    // Compute step size
    h = (b - a) / n;

    // Each process handles n/size intervals
    int local_n = n / size;
    local_a = a + rank * local_n * h;
    local_b = local_a + local_n * h;

    // Start timing
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    // Compute local integral using trapezoidal rule
    double x;
    local_integral = (f(local_a) + f(local_b)) / 2.0;
    for (int i = 1; i < local_n; i++) {
        x = local_a + i * h;
        local_integral += f(x);
    }
    local_integral *= h;

    // Combine all partial integrals
    MPI_Reduce(&local_integral, &total_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Stop timing
    end_time = MPI_Wtime();

    // Root process prints results
    if (rank == 0) {
        printf("-----------------------------------------------------\n");
        printf("MPI Trapezoidal Rule Integration\n");
        printf("Function: f(x) = x^2\n");
        printf("Integration limits: [%.2f, %.2f]\n", a, b);
        printf("Number of intervals: %d\n", n);
        printf("Number of processes: %d\n", size);
        printf("Computed integral: %.6f\n", total_integral);
        printf("Expected result (analytical): %.6f\n", (b*b*b - a*a*a)/3.0);
        printf("Parallel Execution Time: %f seconds\n", end_time - start_time);
        printf("-----------------------------------------------------\n");
    }

    MPI_Finalize();
    return 0;
}
