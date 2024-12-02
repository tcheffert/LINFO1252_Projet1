#!/bin/bash

# Paths to programs compiled
PRODUCERS_CONSUMERS_PROGRAM="./prod_conso_sem"
READERS_WRITERS_PROGRAM="./readers_writers_sem"

# Output repository for CSV files
OUTPUT_REPO="./performance_data"

#Have to be sure that the repo exists
mkdir -p $OUTPUT_REPO

# Output CSV files
PC_OUTPUT_FILE="$OUTPUT_REPO/performance_prod_conso_sem.csv"
RW_OUTPUT_FILE="$OUTPUT_REPO/performance_readers_writers_sem.csv"

# Number of threads
THREAD_COUNTS=(2 4 8 16 32)

# Number of runs per configuration
N=5

# Prepare the CSV files
echo "Threads,Run,Time (s)" > $PC_OUTPUT_FILE
echo "Threads,Run,Time (s)" > $RW_OUTPUT_FILE

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

            # Append results to the CSV file
            echo "$THREADS,$RUN,$FINAL_TIME" >> $output_file
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
