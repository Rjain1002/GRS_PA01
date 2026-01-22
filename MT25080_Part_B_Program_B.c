// I used AI to create this component of file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "MT25080_Part_A_workers.h"

// Argument structure for thread
struct Args {
    int id;
    int iterations;
    char *type;
};

// Function that thread will run
void *worker_func(void *ptr) {
    struct Args *arg = (struct Args *)ptr;
    
    if (strcmp(arg->type, "cpu") == 0) {
        run_cpu_intensive(arg->id, arg->iterations);
    }
    else if (strcmp(arg->type, "mem") == 0) {
        run_memory_intensive(arg->id, arg->iterations);
    }
    else if (strcmp(arg->type, "io") == 0) {
        run_io_intensive(arg->id, arg->iterations);
    }
    
    return NULL;
}

int main(int argc, char *argv[]) {
    // Check arguments
    if (argc < 3) {
        printf("Error: Provide threads and type\n");
        return 1;
    }

    int n = atoi(argv[1]);
    char *type = argv[2];

    // Roll number logic
    int digit = 0; // My last digit
    if (digit == 0) {
        digit = 9;
    }
    int iters = digit * 1000;

    // Arrays for threads and arguments
    pthread_t threads[100]; // Assume max 100 for assignment
    struct Args args[100];
    
    // If n is too big
    if (n > 100) {
        n = 100;
    }

    int i;
    printf("Parent: Creating %d threads...\n", n);
    for (i = 0; i < n; i++) {
        args[i].id = i;
        args[i].iterations = iters;
        args[i].type = type;

        // Create thread
        if (pthread_create(&threads[i], NULL, worker_func, &args[i]) != 0) {
            printf("Error creating thread\n");
        }
    }

    // Join threads
    for (i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Parent: All threads completed.\n");

    return 0;
}
