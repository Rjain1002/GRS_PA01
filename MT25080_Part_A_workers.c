// I used AI to create this component of file
#include "MT25080_Part_A_workers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// CPU Task: Check primary numbers or just some math
// Using simple multiplication and addition loop
void run_cpu_intensive(int id, long long iterations) {
    (void)id; // Silence unused warning
    (void)iterations; // Not used, fixed iterations
    long long i;
    double x = 1.5;

    // Fixed iterations: last digit 0 -> 9, 9 * 1000 = 9000
    long long fixed_iterations = 9000;
    // Use a multiplier to make it run for some time
    long long limit = fixed_iterations * 100000;
    
    for (i = 0; i < limit; i++) {
        // Just some random maths
        x = x * 1.000001 + 0.000001;
        if (x > 1000) {
             x = 1.5;
        }
    }
    
    // Print just to use the variable so compiler doesn't delete loop
    printf("Child %d: CPU Task Finished. Result: %f\n", id, x);
}

// Memory Task: big array and writing reading
void run_memory_intensive(int id, long long iterations) {
    (void)id; // Silence unused warning
    (void)iterations; // Not used, fixed iterations
    // Allocation size 50MB
    int size = 50 * 1024 * 1024;
    int *arr = (int*) malloc(size * sizeof(int));
    
    if (arr == NULL) {
        printf("Memory fail\n");
        return;
    }

    // Fixed iterations: 9
    int k;
    long final_sum = 0;
    for (k = 0; k < 9; k++) {
        int i;
        // Fill array
        for (i = 0; i < size; i++) {
            arr[i] = i;
        }
        
        // Read array
        long sum = 0;
        for (i = 0; i < size; i++) {
            sum = sum + arr[i];
        }
        final_sum = sum;
    }

    printf("Child %d: Memory Task Finished. Sum: %ld\n", id, final_sum);

    free(arr);
}

// IO Task: Writing to files
void run_io_intensive(int id, long long iterations) {
    (void)iterations; 
    char name[50];
    // Make unique filename in /tmp for better I/O capture
    sprintf(name, "/tmp/file_%d.txt", id);

    FILE *f = fopen(name, "w");
    if (f == NULL) return;

    long long i;
    // Fixed iterations: 9000 * 100 = 900,000 writes
    for (i = 0; i < 9000LL * 100; i++) {
        fprintf(f, "Line number %lld is being written here\n", i);
    }
    
    fclose(f);
    
    printf("Child %d: IO Task Finished. Wrote to %s\n", id, name);
    
    // Remove the file at end
    remove(name);
}
