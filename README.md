# PA01: Process vs Thread Performance Analysis

**Name**: Ruchir Jain  
**Roll No**: MT25080

## Overview

This is my assignment to compare Processes and Threads.
I have written programs to check performance of CPU, Memory and IO tasks.

## How to Build

I have made a Makefile. You can just run:

```bash
make
```

This will create `MT25080_Part_A_Program_A` and `MT25080_Part_B_Program_B`.

To clean:

```bash
make clean
```

## How to Run

### Run Manually

For Process:

```bash
./MT25080_Part_A_Program_A <number_of_processes> <type>
```

Type can be: cpu, mem, io

For Thread:

```bash
./MT25080_Part_B_Program_B <number_of_threads> <type>
```

### Run Experiments Script

I have written a script to run everything and save to CSV.

```bash
bash MT25080_Part_C_shell.sh
```

This will create `MT25080_Part_C_CSV.csv` and `MT25080_Part_D_CSV.csv`.
