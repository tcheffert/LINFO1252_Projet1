#!/bin/bash

# Number of threads
THREAD_COUNTS=(2 4 8 16 32)

# Number of runs per configuration
N=5

PHILOSOPHERS_PROGRAM="./philosophers_TATAS"

RESULT_FILE="perf.csv"

# Ensure any existing file is backed up or removed before starting
if [[ -f $RESULT_FILE ]]; then
    mv $RESULT_FILE "${RESULT_FILE}"
    echo "Existing results file backed up as ${RESULT_FILE}.bak"
fi

# Initialize the CSV file with a header
echo "Program,Threads,Run,Time (s)" > $RESULT_FILE

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
            if [[ "$program" == "$PHILOSOPHERS_PROGRAM" ]]; then
                # Measure execution time with /usr/bin/time for philosophers program
                FINAL_TIME=$(/usr/bin/time -f "%E" $program $THREADS 2>&1)
            else
                # Measure execution time with /usr/bin/time for other programs
                FINAL_TIME=$(/usr/bin/time -f "%E" $program $READERS $WRITERS 2>&1)
            fi

            # Display results in the terminal
            #echo "Program: $program | Threads: $THREADS | Run: $RUN | Time: $FINAL_TIME seconds"

            # Append results to the CSV file
            echo "$program,$THREADS,$RUN,$FINAL_TIME" >> $RESULT_FILE
        done
    done
}

# Producers-Consumers
READERS_WRITERS_PROGRAM="./prod_conso_TAS"
measure_performance $READERS_WRITERS_PROGRAM THREAD_COUNTS[@]
READERS_WRITERS_PROGRAM="./prod_conso_TATAS"
measure_performance $READERS_WRITERS_PROGRAM THREAD_COUNTS[@]
READERS_WRITERS_PROGRAM="./producers_consumers"
measure_performance $READERS_WRITERS_PROGRAM THREAD_COUNTS[@]

# Readers-Writers
READERS_WRITERS_PROGRAM="./readers_writers"
measure_performance $READERS_WRITERS_PROGRAM THREAD_COUNTS[@]
READERS_WRITERS_PROGRAM="./readers_writers_TAS"
measure_performance $READERS_WRITERS_PROGRAM THREAD_COUNTS[@]
READERS_WRITERS_PROGRAM="./readers_writers_TATAS"
measure_performance $READERS_WRITERS_PROGRAM THREAD_COUNTS[@]

# Display CSV file content
echo "Results saved in $RESULT_FILE:"
cat $RESULT_FILE

# Clean up the CSV file at the end
rm -f $RESULT_FILE
echo "Temporary results file $RESULT_FILE removed."

echo "Performance evaluation complete."
