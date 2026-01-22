#ifndef WORKERS_H
#define WORKERS_H

#include <stddef.h>

// Exposed worker functions
void run_cpu_intensive(int id, long long iterations);
void run_memory_intensive(int id, long long iterations);
void run_io_intensive(int id, long long iterations);

#endif
