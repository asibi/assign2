#include <stdio.h>
#include "minunit.h"
#include "MergeSort.h"
#include "Parallel.h"

int tests_run = 0;

// todo test init_array

static char * test_merge_sort() {
    // given
    int N = 1000;
    int *x = (int*)malloc(sizeof(int) * N);
    init_array(x, N);

    // when
    merge_sort(x, 0, N-1);

    // then 
    mu_assert_array_sorted("Arrays not equal", N, x);
    return 0;
}

static char * test_parallel_sort() {
    // given
    int N = 100000;
    int *x = (int*)malloc(sizeof(int) * N);
    init_array(x, N);

    // when
    parallel_sort(x, 0, N-1, 8);

    // then 
    mu_assert_array_sorted("Arrays not equal", N, x);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_merge_sort);
    mu_run_test(test_parallel_sort);
    return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}