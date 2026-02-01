#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    int N = 100000000; // Default array size (100 million elements)
    if (argc > 1)
        N = atoi(argv[1]);

    int *array = (int *)malloc(N * sizeof(int));
    if (array == NULL) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    // Initialize random array
    srand(42);
    for (int i = 0; i < N; i++) {
        array[i] = rand() % 1000000; // Random numbers between 0 to 999,999
    }

    double start, end;
    int min_val, max_val;

    // -------------------------------
    // Serial computation
    // -------------------------------
    start = omp_get_wtime();
    min_val = array[0];
    max_val = array[0];
    for (int i = 1; i < N; i++) {
        if (array[i] < min_val) min_val = array[i];
        if (array[i] > max_val) max_val = array[i];
    }
    end = omp_get_wtime();
    double serial_time = end - start;

    printf("-----------------------------------------------------\n");
    printf("Serial Computation:\n");
    printf("Minimum: %d, Maximum: %d\n", min_val, max_val);
    printf("Execution Time (Serial): %f seconds\n", serial_time);
    printf("-----------------------------------------------------\n");

    // -------------------------------
    // Parallel computation (OpenMP)
    // -------------------------------
    int threads = 4; // Default number of threads
    if (argc > 2)
        threads = atoi(argv[2]); // Optional thread count argument

    int global_min = INT_MAX;
    int global_max = INT_MIN;

    omp_set_num_threads(threads);
    start = omp_get_wtime();

    #pragma omp parallel
    {
        int local_min = INT_MAX;
        int local_max = INT_MIN;

        #pragma omp for nowait
        for (int i = 0; i < N; i++) {
            if (array[i] < local_min) local_min = array[i];
            if (array[i] > local_max) local_max = array[i];
        }

        #pragma omp critical
        {
            if (local_min < global_min) global_min = local_min;
            if (local_max > global_max) global_max = local_max;
        }
    }

    end = omp_get_wtime();
    double parallel_time = end - start;

    printf("Parallel Computation (OpenMP with %d threads):\n", threads);
    printf("Minimum: %d, Maximum: %d\n", global_min, global_max);
    printf("Execution Time (Parallel): %f seconds\n", parallel_time);
    printf("-----------------------------------------------------\n");
    printf("Speedup = %.2f\n", serial_time / parallel_time);
    printf("-----------------------------------------------------\n");

    free(array);
    return 0;
}
