#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define cycles 1000000  // Nombre de cycles penser/manger pour chaque philospohe

int N;  // Nombre de philospohes (sera donné par argument plus tard)
pthread_mutex_t *fourchettes;  // Tableau représentant les fourchettes


void* philosophe(void* id) {
    return (NULL);   
}