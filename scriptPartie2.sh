#!/bin/bash

# Paths to programs compiled
PRODUCERS_CONSUMERS_PROGRAM="./prod_conso_TATAS"
READERS_WRITERS_PROGRAM="./readers_writers_TATAS"

# Output repository for CSV files
OUTPUT_REPO="./performance_local"

#Have to be sure that the repo exists
mkdir -p $OUTPUT_REPO

# Output CSV files
PC_OUTPUT_FILE="$OUTPUT_REPO/pc_TATAS.csv"
RW_OUTPUT_FILE="$OUTPUT_REPO/rw_TATAS.csv"

# Number of threads
THREAD_COUNTS=(2 4 8 16 32)

# Number of runs per configuration
N=10

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

            FINAL_TIME=$(/usr/bin/time -f "%E" $program $READERS $WRITERS 2>&1)
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
