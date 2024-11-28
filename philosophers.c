#include "imports.h"

#define cycles 1000000  // Nombre de cycles penser/manger pour chaque philospohe

int N;  // Nombre de philospohes (sera donné par argument plus tard)
// Faire un tableau représentant les fourchettes
pthread_mutex_t* forks; // Tableau de sémaphores pour les fourchettes

void* philosophe(void* arg) {
    int *id = (int *) arg;
    int right = (*id + 1) % N;
    int left = *id;

    int count = 0;
    while ( count < cycles){
        if (left < right){ // penser
            pthread_mutex_lock(&forks[left]);
            pthread_mutex_lock(&forks[right]);
        } else {
            pthread_mutex_lock(&forks[right]);
            pthread_mutex_lock(&forks[left]);
        }
        pthread_mutex_unlock(&forks[left]); // manger
        pthread_mutex_unlock(&forks[right]);
        count++;
    }
    free(arg);  // Libérer la mémoire allouée pour l'identifiant
    return (NULL);   
}

void problem(int N){
    pthread_t *philos = (pthread_t *) malloc(N * sizeof(pthread_t));
    forks = (pthread_mutex_t *) malloc(N * sizeof(pthread_mutex_t));

    //Check malloc
    if (philos == NULL || forks == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    //Check initialisation mutex et threads
    int *id;
    for (int i = 0; i < N; i++){

        if(pthread_mutex_init(&forks[i], NULL) != 0){
            perror("mutex init");
            exit(1);
        }
        id = malloc(sizeof(int));
        *id = i;
        if(pthread_create(&philos[i], NULL, philosophe, (void *) id) != 0){
            perror("Error pthread_create");
            exit(1);
        }
    }

    //Threads philosophes
    for (int i = 0; i < N; i++){
        if(pthread_join(philos[i], NULL) != 0){
            perror("pthread_join");
            exit(1);
        }
    }

    for (int i = 0; i < N; i++){
        if(pthread_mutex_destroy(&forks[i]) == -1){
            perror("Error mutex destroy");
            exit(1);
        }
    }

    free(philos);
    free(forks);
    free(id);
}

int main(int argc, char const *argv[])
{
    /* // uncomment if manual launch
    if (argc != 2){
        printf("Usage: %s <N>\n", argv[0]);
        return -1;
    }
    N = atoi(argv[1]);
    */
    N = 10;
    if (N <= 0){
        printf("Erreur : Le nombre de philosophes (%d) doit être positif!\n", N);
        return -1;
    }
    problem(N);
    return 0;
}