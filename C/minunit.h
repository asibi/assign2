/* file: minunit.h */
#include <limits.h>
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                            if (message) return message; } while (0)
#define mu_assert_array_sorted(message, n, array) do {\
    int current = INT_MIN;\
    for(int i = 0; i < n; i++) {\
        if(array[i] < current){ printf("Failure: %d < %d\n", array[i], current); return message; }\
        current = array[i];\
    }} while (0)
#define mu_assert_equals_arrays(message, n, result, exp) do { \
    for(int i = 0; i < n; i++) {if (result[i] != exp[i]) { printf("%d -%d\n", result[i], exp[i]); return message; }}  \
} while (0)