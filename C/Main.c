#include <stdio.h>
#include <time.h>
#include <stdbool.h>  
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

// return array, always allocate multiple of 2
int* read_data(char *path, int *N) {
    FILE *file;
    file = fopen(path, "r");
    int count = 0; 
    int capacity = 2; 
    int *arr = (int*) malloc(capacity * sizeof(int));

    if(file == NULL) {
        printf("Not able to open the file\n");
        exit(1);
    }

    int num; 
    while(fscanf(file, "%d", &num) == 1) {
        if(count == capacity) {
            capacity *= 2; 
            arr = realloc(arr, capacity * sizeof(int));
        }
        arr[count++] = num;
    }
    *N = capacity; 
    fclose(file);
    
    return arr; 
}

// User arguments: <path to workload file> <num. threads>
int main(int argc, char** argv) {
    // Handle arguments 
    assert(argc == 3);
    char *path = argv[1];
    int nrThreads = atoi(argv[2]);

    // Load data 
    int N; 
    int *x = read_data(path, &N); 
    assert(sizeof(int) == sizeof(int32_t)); // If fails use int32_t

    // Measure time
    measure(parallel_sort, x, 0, N-1, nrThreads);

    // Info
    assert(isSorted(x, N));

    // Clean
    free(x);
}