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
        // thread_count++; // for debugging

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

void parallel_sort(int x[], int start, int end, int nrThreads) {
    int maxThreadDepth = (int)log2(nrThreads);
    parallel_depth(x, start, end, 0, maxThreadDepth); 
}

#endif