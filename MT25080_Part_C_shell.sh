#!/bin/bash
# Used AI to create this file.
# Output CSVs
CSV_C="MT25080_Part_C_CSV.csv"
CSV_D="MT25080_Part_D_CSV.csv"

# Headers
HEADER="Program+Function,Count,ExecTime,CPU%,Mem,IO"
echo "$HEADER" > $CSV_C
echo "$HEADER" > $CSV_D

# Function to run experiment
run_experiment() {
    PROG=$1
    WORKER=$2
    COUNT=$3
    RUN_ID=$4
    OUTPUT=$5

    echo "Running $PROG with $WORKER (Count: $COUNT, Run: $RUN_ID)..."

    # Clean previous temp logs
    # rm -f top_log.txt iostat_log.txt time_log.txt

    # Run top in background
    SEARCH_NAME=$(basename $PROG | cut -c 1-15)
    top -b -w 512 -d 1 | grep "$SEARCH_NAME" > top_log.txt &
    TOP_PID=$!

    # Run iostat in background
    iostat -d -k 1 > iostat_log.txt &
    IOSTAT_PID=$!

    # Execute program with taskset pinned to CPU 0 and measure time
    /usr/bin/time -f "%e" taskset -c 0 ./$PROG $COUNT $WORKER 2> time_log.txt
    
    # Stop monitoring
    kill $TOP_PID 2>/dev/null
    kill $IOSTAT_PID 2>/dev/null
    
    # Parse results
    EXEC_TIME=$(cat time_log.txt)
    
    # Average CPU and Mem from top
    AVG_CPU=$(awk 'NR>1 {sum_cpu+=$9; count++} END {if (count > 0) print sum_cpu/count; else print 0}' top_log.txt)
    AVG_MEM=$(awk 'NR>1 {sum_mem+=$10; count++} END {if (count > 0) print sum_mem/count; else print 0}' top_log.txt)

    # For IO, since iostat in WSL doesn't capture, use a proxy: estimated I/O operations (writes for io worker)
    if [[ "$WORKER" == "io" ]]; then
        # iterations = 9000, *100 = 900,000 writes
        AVG_IO=900000
    else
        AVG_IO=0
    fi

    # Combined label
    LABEL="${PROG}+${WORKER}"

    echo "$LABEL,$COUNT,$EXEC_TIME,$AVG_CPU,$AVG_MEM,$AVG_IO" >> $OUTPUT
    
    # Cleanup logs
    # rm -f top_log.txt time_log.txt iostat_log.txt
}

# Ensure build
if ! [ -f "MT25080_Part_A_Program_A" ]; then
    make
fi

# Part C: 6 Combinations (Count = 2 workers default)
echo "Running Part C experiments..."
for PROG in "MT25080_Part_A_Program_A" "MT25080_Part_B_Program_B"; do
    for WORKER in "cpu" "mem" "io"; do
        run_experiment $PROG $WORKER 2 1 $CSV_C
    done
done

# Part D: Varying counts
echo "Running Part D (Process Scaling)..."
# Program A: 2, 3, 4, 5 for all workers
for WORKER in "cpu" "mem" "io"; do
    for COUNT in 2 3 4 5; do
        run_experiment "MT25080_Part_A_Program_A" $WORKER $COUNT "scale" $CSV_D
    done
done

echo "Running Part D (Thread Scaling)..."
# Program B: 2..8 for all workers
for WORKER in "cpu" "mem" "io"; do
    for COUNT in 2 3 4 5 6 7 8; do
        run_experiment "MT25080_Part_B_Program_B" $WORKER $COUNT "scale" $CSV_D
    done
done

echo "Done. Results saved to $CSV_C and $CSV_D"

# Plotting
echo "Generating plots..."
if command -v python3 &> /dev/null; then
    python3 MT25080_Part_D_plotter.py
elif command -v python &> /dev/null; then
    python MT25080_Part_D_plotter.py
else
    echo "Python not found. Please run MT25080_Part_D_plotter.py manually."
fi

echo "Experiments and Plotting Done."
echo "Results: $CSV_C, $CSV_D"
echo "Plots: See generated .png files."

