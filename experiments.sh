#!/bin/bash

# Paths to programs compiled
PRODUCERS_CONSUMERS_PROGRAM="./prod_conso_sem"
READERS_WRITERS_PROGRAM="./readers_writers_sem"

# Number of threads
THREAD_COUNTS=(2 4 8 16 32)

# Number of runs per configuration
N=5

# Compile programs
echo "Compiling programs..."
make all

# Function to measure performance for a given program
measure_performance() {
    local program=$1
    local thread_counts=("${!2}")

    echo "Evaluating $program..."

    # Loop through each thread count
    for THREADS in "${thread_counts[@]}"; do
        READERS=$((THREADS / 2))
        WRITERS=$((THREADS - READERS)) # Ensure sum matches THREADS

        # Run the program multiple times for each configuration
        for ((RUN=1; RUN<=N; RUN++)); do
            START_TIME=$(date +%s.%N)
            $program $READERS $WRITERS > /dev/null 2>&1
            END_TIME=$(date +%s.%N)
            FINAL_TIME=$(echo "$END_TIME - $START_TIME" | bc)

            # Display results in the terminal
            echo "Program: $program | Threads: $THREADS | Run: $RUN | Time: $FINAL_TIME seconds"
        done
    done
}

# Measure performance for producers/consumers
measure_performance $PRODUCERS_CONSUMERS_PROGRAM THREAD_COUNTS[@]

# Measure performance for readers/writers
measure_performance $READERS_WRITERS_PROGRAM THREAD_COUNTS[@]

# Clean up build files
echo "Cleaning up..."
make clean
if [[ $? -ne 0 ]]; then
    echo "Warning: Failed to clean build files." >&2
fi

echo "Performance evaluation complete."
