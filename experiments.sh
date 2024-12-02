#!/bin/bash

# Paths to programs compiled
PRODUCERS_CONSUMERS_PROGRAM="./prod_conso_sem"
READERS_WRITERS_PROGRAM="./readers_writers_sem"

# Number of threads
THREAD_COUNTS=(2 4 8 16 32)

# Number of runs per configuration
N=5

make all

# Function to measure performance for a given program
measure_performance() {
    local program=$1
    local output_file=$2
    local thread_counts=("${!3}")

    # Loop through each thread count 
    for THREADS in "${thread_counts[@]}"; do
        READERS=$((THREADS / 2))
        WRITERS=$((THREADS - READERS)) # Ensure sum matches THREADS

        # Run the program multiple times for each configuration
        for ((RUN=1; RUN<=N; RUN++)); do
            # Measure the execution time
            START_TIME=$(date +%s.%N)
            
            # Run the program with suppressed output
            $program $READERS $WRITERS > /dev/null 2>&1
            
            END_TIME=$(date +%s.%N)

            # Calculate elapsed time
            FINAL_TIME=$(echo "$END_TIME - $START_TIME" | bc)

            # Display results in the terminal
            echo "Program: $program | Threads: $THREADS | Run: $RUN | Time: $FINAL_TIME seconds"
        done
    done
}

echo "Starting performance evaluation..."

# Measure performance for producers/consumers
echo "Evaluating Producers/Consumers..."
measure_performance $PRODUCERS_CONSUMERS_PROGRAM $PC_OUTPUT_FILE THREAD_COUNTS[@]

# Measure performance for readers/writers
echo "Evaluating Readers/Writers..."
measure_performance $READERS_WRITERS_PROGRAM $RW_OUTPUT_FILE THREAD_COUNTS[@]

echo "Performance evaluation complete."

make clean
