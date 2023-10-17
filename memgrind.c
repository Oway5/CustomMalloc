#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

#define NUM_TRIALS 50
#define NUM_ALLOCATIONS 15

int main() {
    struct timeval start, end;
    double time_spent[5][NUM_TRIALS] = {{0.0}};

    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        // Task 1 - malloc() and immediately free() a 1-byte object, 120 times
        gettimeofday(&start, NULL);
        for (int i = 0; i < NUM_ALLOCATIONS; i++) {
            char *ptr = (char *)mymalloc(1, __FILE__, __LINE__);
            myfree(ptr, __FILE__, __LINE__);
        }
        gettimeofday(&end, NULL);
        time_spent[0][trial] = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

        // Task 2 - Use malloc() to get 120 1-byte objects, storing the pointers
        //  in an array, then use free() to deallocate the chunks
        gettimeofday(&start, NULL);
        char *pointers[NUM_ALLOCATIONS];
        for (int i = 0; i < NUM_ALLOCATIONS; i++) {
            pointers[i] = (char *)mymalloc(1, __FILE__, __LINE__);
        }
        for (int i = 0; i < NUM_ALLOCATIONS; i++) {
            myfree(pointers[i], __FILE__, __LINE__);
        }
        gettimeofday(&end, NULL);
        time_spent[1][trial] = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

        // Task 3 - Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte
        // object and adding the pointer to the array and deallocating a previously allocated object (if
        // any), until you have allocated 120 times. Deallocate any remaining objects.
        gettimeofday(&start, NULL);
        char *random_pointers[NUM_ALLOCATIONS];
        int num_allocs = 0;
        int num_frees = 0;
        while (num_allocs < NUM_ALLOCATIONS) {
            if (rand() % 2 == 0 || num_allocs == num_frees) {
                random_pointers[num_allocs] = (char *)mymalloc(1, __FILE__, __LINE__);
                num_allocs++;
            } else {
                myfree(random_pointers[num_frees], __FILE__, __LINE__);
                num_frees++;
            }
        }
        for (int i = num_frees; i < NUM_ALLOCATIONS; i++) {
            myfree(random_pointers[i], __FILE__, __LINE__);
        }
        gettimeofday(&end, NULL);
        time_spent[2][trial] = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

        // Task 4 - Allocate 7 byte objects repeatedly 120 times and free them.

        gettimeofday(&start, NULL);
        for (int i = 0; i < NUM_ALLOCATIONS; i++) {
            char *ptr = (char *)mymalloc(7, __FILE__, __LINE__);
            myfree(ptr, __FILE__, __LINE__);
        }
        gettimeofday(&end, NULL);
        time_spent[3][trial] = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

        // Task 5 - Allocate a value to my pointer from a variable and print it.

        gettimeofday(&start, NULL);

        char *ptr = (char *)mymalloc(20, __FILE__, __LINE__);
        *ptr = trial;
        printf("TASK 5 Value in Local Var & Malloc Arr: %d and %d \n", trial, (int)(*ptr));

        myfree(ptr, __FILE__, __LINE__);

        gettimeofday(&end, NULL);
        time_spent[4][trial] = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    }

    // Calculate and print average times for each task
    printf("Average times for each task (in seconds):\n");
    for (int task = 0; task < 5; task++) {
        double sum = 0.0;
        for (int trial = 0; trial < NUM_TRIALS; trial++) {
            sum += time_spent[task][trial];
        }
        printf("Task %d: %.6f\n", task + 1, sum / NUM_TRIALS);
    }

    return 0;
}
