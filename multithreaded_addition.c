/*****************************************************************************
/ PROGRAM:  multithreaded_addition.c
/ AUTHOR:      ****  YOUR NAME HERE ****
/ COURSE:   CS431 - Operating Systems
/ SYNOPSIS: Creates an array of 10000 ints, then populates it with pseudorandom
/           integers between 0 and 100.  Creates a single thread that computes
/           the sum of all ints in the array.  Your assigment is to modify this
/           code so that the solution is divided among five concurrent threads.
/
/           To build, you must manually link the pthread library:
/
/           gcc -o multithreaded_addition multithreaded_addition.c -lpthread 
/
/           (c) Regis University
/*****************************************************************************/
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_COUNT 10000
#define NUM_THREADS 5
#define COUNT_PER_THREAD 

// Define a data structure that contains the indices representing the bounds
// of the array to add together.  The sum field contains the sum of the numbers
// within the bounds of the array between 'begin' and 'end'. Placing this field
// inside the struct, which is accessible from the main program, is easier than
// getting the return value of the thread function. 

struct limits {
    int begin;
    int end;
    int sum;
};

// The array containing NUM_COUNT number of integers to add together

int numbers[NUM_COUNT];
int results[NUM_THREADS];

// The thread function that adds the numbers in the array starting with the
// begin field of the args struct (note it's a void* cast to a (struct limits*))
// through the index represented by the end field (inclusive). 

void* add_numbers(void* args) {

    int first = ((struct limits*) args)->begin;
    int last = ((struct limits*) args)->end;

    for (int i = first; i <= last; i++)
        ((struct limits*) args)->sum += numbers[i];
}

void* thread_add(void* args) {
    int start = (int) args * (NUM_COUNT / NUM_THREADS);
    int end = start + (NUM_COUNT / NUM_THREADS);
    results[(int) args] = 0;
    printf("Thread %i beginning. Processing index %i - %i\n", (int) args, start, end);
    for (int i = start; i < end; i++) {
        results[(int) args] += numbers[i];
        //printf("numbers[%i]: %i\n",i, numbers[i]);
    }
    printf("Thread %i completed its run. Processed %i entries. Subset Sum: %i\n",
            (int) args, end - start, results[(int) args]);
}

int main(int argc, char** argv) {

    // Dynamically allocate memory for and initialize fields of a struct that 
    // contains the array indices from which to start and end when adding ints. 
    //struct limits* one = (struct limits*)malloc(sizeof(struct limits));
    //one->begin = 0;
    //one->end = NUM_COUNT-1;
    int correct_total = 0;
    int calculated_total = 0;

    // Seed the pseudorandom number generator with the current system time
    srand(time(0));

    // Populate the numbers array with pseudorandom ints between 1 and 100
    // and tally the correct total so we can check for program correctness

    for (int i = 0; i < NUM_COUNT; i++) {
        //printf("Current Total: %i\n", correct_total);
        numbers[i] = (rand() % 100) + 1;
        correct_total += numbers[i];
        //printf("Numbers[%i]: %i\n", i, numbers[i]);
        // printf("Current Total: %i\n", correct_total);

    }

    printf("The correct sum is: %d\n", correct_total);


    // Create a single thread to sum all numbers.  Your job is to divide the
    // addition task among five threads, each handling a subset of the array. 
    //pthread_t thread_num;
    //pthread_create(&thread_num, NULL, add_numbers, (void*)one);
    //pthread_join(thread_num, NULL);

    //printf("Threaded sum is: %d\n", one->sum);
    pthread_t thread[NUM_THREADS];
    struct limits* limits_Array[NUM_THREADS] = (struct limits*) malloc(sizeof (struct limits) * NUM_THREADS);



    for (int i = 0; i < NUM_THREADS; i++) {
        //pthread_create(&thread[i], NULL, thread_add, (void *) i);
        
        
        
        
        pthread_create(&thread[i], NULL, add_numbers, (void *)limits_Array[i]);

    }



    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread[i], NULL);
        calculated_total += results[i];
    }

    printf("Correct Total: %i\n", correct_total);
    printf("Calculated Total: %i\n", calculated_total);

    return EXIT_SUCCESS;
}
