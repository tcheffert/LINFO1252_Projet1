#!/bin/bash

# Number of threads
THREAD_COUNTS=(2 4 8 16 32)

# Number of runs per configuration
N=5

PHILOSOPHERS_PROGRAM="./philosophers_TATAS"

# Compile programs
echo "Compiling programs..."
make clean
make all

# Function to measure performance for a given program
measure_performance() {
    local program=$1
    local thread_counts=("${!2}")

    echo "Evaluating $program..."
    bc_scale=9

    # Loop through each thread count
    for THREADS in "${thread_counts[@]}"; do
        READERS=$((THREADS / 2))
        WRITERS=$((THREADS - READERS)) # Ensure sum matches THREADS

        # Run the program multiple times for each configuration
        for ((RUN=1; RUN<=N; RUN++)); do
            START_TIME=$(date +%s.%N)

            if [[ "$program" == "$PHILOSOPHERS_PROGRAM" ]]; then
                $program $THREADS > /dev/null 2>&1
                #valgrind --leak-check=yes $program $THREADS
            else
                $program $READERS $WRITERS > /dev/null 2>&1
                #valgrind --leak-check=yes $program $READERS $WRITERS
            fi

            END_TIME=$(date +%s.%N)
            FINAL_TIME=$(echo "scale=$bc_scale; $END_TIME - $START_TIME" | bc)

            # Display results in the terminal
            echo "Program: $program | Threads: $THREADS | Run: $RUN | Time: $FINAL_TIME seconds"
        done
    done
}

# Producers Consumers
PRODUCERS_CONSUMERS_PROGRAM="./prod_conso_TATAS"
measure_performance $PRODUCERS_CONSUMERS_PROGRAM THREAD_COUNTS[@]
PRODUCERS_CONSUMERS_PROGRAM="./prod_conso_TAS"
measure_performance $PRODUCERS_CONSUMERS_PROGRAM THREAD_COUNTS[@]
PRODUCERS_CONSUMERS_PROGRAM="./producers_consumers"
measure_performance $PRODUCERS_CONSUMERS_PROGRAM THREAD_COUNTS[@]

# Readers Writers
READERS_WRITERS_PROGRAM="./readers_writers_TATAS"
measure_performance $READERS_WRITERS_PROGRAM THREAD_COUNTS[@]
READERS_WRITERS_PROGRAM="./readers_writers_TAS"
measure_performance $READERS_WRITERS_PROGRAM THREAD_COUNTS[@]
READERS_WRITERS_PROGRAM="./readers_writers"
measure_performance $READERS_WRITERS_PROGRAM THREAD_COUNTS[@]

# Philosophers
PHILOSOPHERS_PROGRAM="./philosophers_TATAS"
measure_performance $PHILOSOPHERS_PROGRAM THREAD_COUNTS[@]
PHILOSOPHERS_PROGRAM="./philosophers_TAS"
measure_performance $PHILOSOPHERS_PROGRAM THREAD_COUNTS[@]
PHILOSOPHERS_PROGRAM="./philosophers"
measure_performance $PHILOSOPHERS_PROGRAM THREAD_COUNTS[@]

echo "Performance evaluation complete."
