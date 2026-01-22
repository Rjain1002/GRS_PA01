// I used AI to create this component of file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "MT25080_Part_A_workers.h"

int main(int argc, char *argv[]) {
    // Check if arguments are provided
    if (argc < 3) {
        printf("Error: Please provide number of processes and worker type\n");
        printf("Usage: ./MT25080_Part_A_Program_A <num> <type>\n");
        exit(1);
    }

    // Convert string argument to integer
    int num_process = atoi(argv[1]);
    char *type = argv[2];

    // Logic for roll number
    // My roll number ends in 80. Last digit is 0. 
    // Thats why i have used 9 instead of 0
    int digit = 9; // Last digit
    
    // Multiply by 1000 as per common logic
    long long int iterations = digit * 1000;

    // Create processes
    printf("Parent: Creating %d %s processes...\n", num_process, type);
    fflush(stdout); // Flush buffer to prevent children from inheriting printed data
    for (int i = 0; i < num_process; i++) {
        int pid = fork();

        if (pid < 0) {
            printf("Fork failed\n");
            exit(1);
        }
        else if (pid == 0) {
            // This is child process
            printf("Child %d: Started\n", i);
            if (strcmp(type, "cpu") == 0) {
                run_cpu_intensive(i, iterations);
            }
            else if (strcmp(type, "mem") == 0) {
                run_memory_intensive(i, iterations);
            }
            else if (strcmp(type, "io") == 0) {
                run_io_intensive(i, iterations);
            }
            else {
                printf("Wrong type\n");
                exit(1);
            }
            
            // Child must exit
            exit(0);
        }
    }

    // specific instructions say parent process is not considered for count
    // Parent waits for all children to finish
    int j;
    for (j = 0; j < num_process; j++) {
        wait(NULL);
    }
    printf("Parent: All children completed.\n");

    return 0;
}
