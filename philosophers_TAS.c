#include "headers/TAS.h" // Ensure TAS.h contains lock_t structure and function definitions

#define CYCLES 1000000 // Number of cycles for thinking/eating

int N; // Number of philosophers
int *forks; // Array of locks representing forks

void *philosophe(void *arg)
{
    int id = *(int *)arg; 
    int right = (id + 1) % N;
    int left = id;

    int count = 0;
    while (count < CYCLES)
    {
        // Attempt to pick forks in order
        if (left < right)
        {
            lock_lock(forks[left]);
            lock_lock(forks[right]);
        }
        else
        {
            lock_lock(forks[right]);
            lock_lock(forks[left]);
        }

        // Simulate eating
        unlock_lock(forks[left]);
        unlock_lock(forks[right]);

        count++;
    }

    free(arg); // Free allocated ID
    return NULL;
}

void problem(int N)
{
    pthread_t *philos = malloc(N * sizeof(pthread_t));
    forks = malloc(N * sizeof(lock_t *));

    if (philos == NULL || forks == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize locks and create threads
    for (int i = 0; i < N; i++)
    {
        forks[i] = malloc(sizeof(lock_t)); // Allocate memory for each lock
        if (forks[i] == NULL || init_lock(forks[i]) != 0)
        {
            perror("Lock initialization failed");
            exit(EXIT_FAILURE);
        }

        int *id = malloc(sizeof(int)); // Allocate ID for thread
        if (id == NULL)
        {
            perror("Memory allocation for ID failed");
            exit(EXIT_FAILURE);
        }

        *id = i;
        if (pthread_create(&philos[i], NULL, philosophe, id) != 0)
        {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < N; i++)
    {
        if (pthread_join(philos[i], NULL) != 0)
        {
            perror("Thread join failed");
            exit(EXIT_FAILURE);
        }
    }

    // Clean up
    for (int i = 0; i < N; i++)
    {
        free_lock(forks[i]);
        free(forks[i]);
    }

    free(philos);
    free(forks);
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <N>\n", argv[0]);
        return EXIT_FAILURE;
    }

    N = atoi(argv[1]);
    if (N <= 0)
    {
        printf("Error: Number of philosophers (%d) must be positive!\n", N);
        return EXIT_FAILURE;
    }

    problem(N);
    return EXIT_SUCCESS;
}
