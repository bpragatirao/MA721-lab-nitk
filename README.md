# Scalable Systems & Parallel Computing Lab 🚀

This repository contains implementations of fundamental parallel computing algorithms using **MPI (Message Passing Interface)** and **OpenMP (Open Multi-Processing)**. The focus is on optimizing computational tasks by distributing workloads across multiple processes and threads.

## 📌 Overview
The assignments explore data parallelism, collective communication, and synchronization in distributed and shared-memory environments.

## 🛠️ Technologies
* **C / C++**
* **MPI:** For distributed memory parallelism (running on multiple nodes/processes).
* **OpenMP:** For shared memory parallelism (multi-threading on a single CPU).

## 📂 Lab Assignments

### 🔹 MPI (Message Passing Interface)
Programs designed for distributed systems where processes communicate via messages.
* **`marks_histogram_mpi.c`**: Parallel frequency distribution of data.
* **`min_max_mpi.c`**: Distributed search for extrema across multiple processes.
* **`trapezoidal_mpi.c`**: Numerical integration using the Trapezoidal rule with process-level workload splitting.
* **`max_reduce.c`**: Implementation of global reduction operations to find maximum values.

### 🔹 OpenMP (Shared Memory)
Programs utilizing multi-threading on multi-core processors.
* **`min_max_openmp.c`**: Finding minimum and maximum values using thread-based synchronization.
* **`trapezoid_openmp.c`**: Parallelizing the Trapezoidal rule for numerical integration using OpenMP directives.

## 🚀 Key Parallel Patterns

### 1. Collective Communication (MPI)
Used in `max_reduce.c` and `min_max_mpi.c` to synchronize data across all nodes.

### 2. Parallel For-Loops (OpenMP)
Used in `trapezoid_openmp.c` to split iterations across threads.
```c
#pragma omp parallel for reduction(+:approx)
for (int i = 1; i <= n-1; i++) {
    approx += f(a + i*h);
}
```
