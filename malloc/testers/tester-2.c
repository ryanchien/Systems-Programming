/**
 * Malloc Lab
 * CS 241 - Spring 2019
 */
 
#include "tester-utils.h"

#define TOTAL_ALLOCS 5 * M

int main() {
    malloc(1);

    int i;
    int **arr = malloc(TOTAL_ALLOCS * sizeof(int *));
    if (arr == NULL) {
        fprintf(stderr, "Memory failed to allocate!\n");
        return 1;
    }

	//fprintf(stdout, "arr: %p\n", arr);

    for (i = 0; i < TOTAL_ALLOCS; i++) {
        arr[i] = malloc(sizeof(int));
        if (arr[i] == NULL) {
            fprintf(stderr, "Memory failed to allocate!\n");
            return 1;
        }
		//fprintf(stdout, "arr[%d]: %p\n", i, arr[i]);
        *(arr[i]) = i;
    }

	//fprintf(stdout, "TOT_ALLOCS: %ld\n", TOTAL_ALLOCS);

    for (i = 0; i < TOTAL_ALLOCS; i++) {
        if (*(arr[i]) != i) {
            fprintf(stderr, "Memory failed to contain correct data after many "
                            "allocations!\n");
            return 2;
        }
    }

    for (i = 0; i < TOTAL_ALLOCS; i++)
        free(arr[i]);

    free(arr);
    fprintf(stderr, "Memory was allocated, used, and freed!\n");
    return 0;
}
