#include "headers/semaphore_interface.h"
#include "headers/imports.h"
#include "headers/TAS.h"

#define cycles 1000000 // Nombre de cycles penser/manger pour chaque philospohe

int N;                  // Nombre de philospohes (sera donné par argument plus tard)

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
            lock_lock(&locks[left]);
            lock_lock(&locks[right]);
        }
        else
        {
            lock_lock(&locks[right]);
            lock_lock(&locks[left]);
        }
        // Graille
        unlock_lock(&locks[left]);
        unlock_lock(&locks[right]);
        count++;
    }
    // printf("Philosophe %d a fini de manger, count : %d \n", *id,count);
    free(id); // Libérer la mémoire allouée pour l'identifiant
    return NULL;
}

void problem(int N)
{
    pthread_t *philos = (pthread_t *)malloc(N * sizeof(pthread_t));
    locks = init_lock(N);

    // Check malloc
    if (philos == NULL || locks == NULL)
    {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    // Check initialisation threads
    for (int i = 0; i < N; i++)
    {

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
        // free(id);
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

    free(philos);
    free(locks);
}

int main(int argc, char const *argv[])
{
    // uncomment if manual launch
    // valgrind --leak-check=full --show-leak-kinds=all ./philosophers_sem 100

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