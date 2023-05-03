#include <stdio.h>
#include <time.h>
#include<stdbool.h>  
#include <assert.h>
#include "MergeSort.h"
#include "Parallel.h"


bool isSorted(int x[], int N) {
    int prev = x[0];
    for(int i = 1; i < N; i++) {
        if(prev > x[i])
            return false;
    }
    return true;
}

double measure(void (*f)(int*, int, int, int), int* x, int start, int end, int nrThreads) {
    struct timespec start_time, end_time;
    // Start timer
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // Run
    (*f)(x, start, end, nrThreads);
    
    // End timer
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    // Handle time
    double time_spent_ms = (end_time.tv_sec - start_time.tv_sec) * 1000;
    time_spent_ms += (end_time.tv_nsec - start_time.tv_nsec) / 1000000.0;
    double time_spent = time_spent_ms;
    printf("%.3f ms\n", time_spent);
    return time_spent;
}


int main(int argc, char** argv) {
    assert(argc == 2);

    // Setup
    int nrThreads = atoi(argv[1]);
    int N = 10000000;
    int* x = (int*) malloc(N * sizeof(int));
    init_array(x, N);

    // Measure time
    measure(parallel_sort, x, 0, N-1, nrThreads);

    // Info
    assert(isSorted(x, N));

    // Clean
    free(x);
}