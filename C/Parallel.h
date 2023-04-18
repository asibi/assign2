#ifndef PARALLEL_H
#define PARALLEL_H

#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include "MergeSort.h"
#include <stdatomic.h>

void parallel_depth(int x[], int start, int end, int depth, int maxThreadDepth);

typedef struct {
    int* x;
    int start;
    int end;
    int depth; 
    int maxThreadDepth;
} ThreadArgs;

void* thread_merge_depth(void* args) {
    ThreadArgs *a = (ThreadArgs*) args; 
    parallel_depth(a->x, a->start, a->end, a->depth, a->maxThreadDepth);
    return NULL;
}

_Atomic int thread_count = 1;

void print_nr_threads() {
    printf("Threads: %d\n", thread_count);
}



void parallel_depth(int x[], int start, int end, int depth, int maxThreadDepth) {
    if(depth < maxThreadDepth) {
        pthread_t t;
        ThreadArgs *a = (ThreadArgs*)malloc(sizeof(ThreadArgs));
        int mid = start + (end-start) / 2;
        // run thread 1
        a->x = x;
        a->start = start;
        a->end = mid; 
        a->depth = depth+1;
        a->maxThreadDepth=maxThreadDepth;
        pthread_create(&t, NULL, thread_merge_depth, (void*)a);
        thread_count++; 

        // run thread 2
        parallel_depth(x, mid+1, end, depth+1, maxThreadDepth);
        pthread_join(t, NULL);

        // Merge
        merge(x, start, mid, end);
    } else {
        // printf("Running1\n");
        merge_sort(x, start, end);
    }
} 

void* thread_merge(void* args) {
    ThreadArgs *a = (ThreadArgs*) args; 
    merge_sort(a->x, a->start, a->end);
    return NULL;
}

void parallel_subarray(int x[], int start, int end, int nrThreads) {
    pthread_t threads[nrThreads];
    ThreadArgs *thread_args = (ThreadArgs*)malloc(sizeof(ThreadArgs) * nrThreads); 
    int full_len = end - start; 
    int len = full_len / nrThreads; 
    for(int t = 0; t < nrThreads; t++) {
        ThreadArgs *args = thread_args + t; 
        args->x = x;
        args->start = len * t;
        args->end = (t == nrThreads-1) ? end : (args->start + len) - 1;
        pthread_create(&threads[t], NULL, thread_merge, (void*)args);
    }

    for(int t = 0; t < nrThreads; t++) {
        pthread_join(threads[t], NULL);
    }
    for(int t = 0; t < nrThreads; t++) {
        int t_start = t * len; 
        int t_end = (t == nrThreads-1) ? end : (t_start + len) -1;
        merge(x, start, t_start, t_end);
    }

    free(thread_args);
} 

void parallel_sort(int x[], int start, int end, int nrThreads) {
    int maxThreadDepth = (int)log2(nrThreads);
    parallel_depth(x, start, end, 0, maxThreadDepth); 
}

#endif