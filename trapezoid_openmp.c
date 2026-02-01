#include <stdio.h>
#include <omp.h>

double f(double x) {
    // Example function: f(x) = x^2
    return x * x;
}

int main(int argc, char *argv[]) {
    double a = 0.0, b = 10.0;      // Interval [a, b]
    long long int N = 100000000;    // Default number of trapezoids
    int threads = 4;               // Default number of threads

    if (argc > 1) N = atoll(argv[1]);       // Number of trapezoids
    if (argc > 2) threads = atoi(argv[2]);  // Number of threads

    double h = (b - a) / (double)N;        // Width of each trapezoid
    double area_serial = 0.0, area_parallel = 0.0;
    double start, end;

    printf("-----------------------------------------------------\n");
    printf("Computing area under curve f(x) = x^2 using trapezoidal rule\n");
    printf("Interval: [%.2f, %.2f], Number of trapezoids: %lld\n", a, b, N);
    printf("Number of threads: %d\n", threads);
    printf("-----------------------------------------------------\n");

    // -------------------------------
    // Serial computation
    // -------------------------------
    start = omp_get_wtime();

    area_serial = 0.5 * (f(a) + f(b));
    for (long long int i = 1; i < N; i++) {
        double x = a + i * h;
        area_serial += f(x);
    }
    area_serial *= h;

    end = omp_get_wtime();
    double serial_time = end - start;

    printf("Serial Area = %.12f\n", area_serial);
    printf("Serial execution time = %f seconds\n", serial_time);
    printf("-----------------------------------------------------\n");

    // -------------------------------
    // Parallel computation (OpenMP)
    // -------------------------------
    omp_set_num_threads(threads);
    start = omp_get_wtime();

    area_parallel = 0.0;
    #pragma omp parallel for reduction(+:area_parallel)
    for (long long int i = 1; i < N; i++) {
        double x = a + i * h;
        area_parallel += f(x);
    }
    area_parallel += 0.5 * (f(a) + f(b));
    area_parallel *= h;

    end = omp_get_wtime();
    double parallel_time = end - start;

    printf("Parallel Area = %.12f\n", area_parallel);
    printf("Parallel execution time = %f seconds\n", parallel_time);
    printf("Speedup = %.2f×\n", serial_time / parallel_time);
    printf("-----------------------------------------------------\n");

    return 0;
}
