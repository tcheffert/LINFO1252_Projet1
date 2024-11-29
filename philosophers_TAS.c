#include "headers/TAS.h"

#define cycles 1000000 // Nombre de cycles penser/manger pour chaque philospohe

int N;                  // Nombre de philospohes (sera donné par argument plus tard)
lock_t *forks; // Tableau de sémaphores représentant les fourchettes

void *philosophe(void *arg)
{
    int *id = (int *)arg;
    int right = (*id + 1) % N;
    int left = *id;

    int count = 0;
    while (count < cycles)
    {
        if (left < right)
        { // penser
            lock_lock(&forks[left]);
            lock_lock(&forks[right]);
        }
        else
        {
            lock_lock(&forks[right]);
            lock_lock(&forks[left]);
        }
        // Graille
        unlock_lock(&forks[left]);
        unlock_lock(&forks[right]);
        count++;
    }
    // printf("Philosophe %d a fini de manger, count : %d \n", *id,count);
    free(id); // Libérer la mémoire allouée pour l'identifiant
    return NULL;
}

void problem(int N)
{
    pthread_t *philos = (pthread_t *)malloc(N * sizeof(pthread_t));
    forks = (lock_t *)malloc(N * sizeof(lock_t));

    // Check malloc
    if (philos == NULL || forks == NULL)
    {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    // Check initialisation mutex et threads
    for (int i = 0; i < N; i++)
    {

        if (init_lock(&forks[i]) != 0)
        {
            perror("Error lock init");
            exit(1);
        }

        int *id = malloc(sizeof(int));
        if (id == NULL)
        {
            perror("Erreur d'alloc de mémoire pour l'id (void problem(int N))");
            exit(EXIT_FAILURE);
        }

        *id = i;
        // L'id sera libéré par le thread dans philosophe() (ligne 33)
        if (pthread_create(&philos[i], NULL, philosophe, id) != 0)
        {
            perror("Error pthread_create");
            exit(1);
        }
    }

    // Threads philosophes
    for (int i = 0; i < N; i++)
    {
        if (pthread_join(philos[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        free_lock(&forks[i]);
    }

    free(philos);
    free(forks);
}

int main(int argc, char const *argv[])
{
    // valgrind --leak-check=yes ./philosophers_TAS 100
    // ./philosophers_TAS 100

    if (argc != 2)
    {
        printf("Usage: %s <N>\n", argv[0]);
        return -1;
    }
    N = atoi(argv[1]);

    // N = 10;
    if (N <= 0)
    {
        printf("Erreur : Le nombre de philosophes (%d) doit être positif!\n", N);
        return -1;
    }
    problem(N);
    return 0;
}