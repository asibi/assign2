#ifndef MERGE_SORT_H
#define MERGE_SORT_H
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <pthread.h>

void print_array(int* x, int N) {
    for(int i = 0; i < N; i++) 
        printf("%d ", x[i]);
    printf("\n");
}

void merge(int x[], int start, int mid, int end) {
    int i,j,k;
    int n1 = (mid - start) + 1;
    int n2 = end - mid;
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));
    
    // Copy arrays
    memcpy(L, x + start,    n1 * sizeof(int));
    memcpy(R, x + mid + 1,  n2 * sizeof(int));

    i = 0; 
    j = 0;
    k = start; 

    while(i < n1 && j < n2) {
        if(L[i] < R[j]) 
            x[k++] = L[i++];
        else 
            x[k++] = R[j++];
    }

    while(i < n1) {
        x[k++] = L[i++];
    }
    while(j < n2) {
        x[k++] = R[j++];
    }
    free(R);
    free(L);
}

void merge_sort(int x[], int start, int end) {
    
    if(start < end) {
        // Divide
        int mid = start + (end-start) / 2;
        merge_sort(x, start, mid);
        merge_sort(x, mid + 1, end);

        // Merge
        merge(x, start, mid, end);
    }
} 



void init_array(int* x, int N) {
    for(int i = 0; i < N; i++) 
        x[i] = rand() % 1000000;
}

#endif // MERGE_SORT_H