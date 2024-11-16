#include "imports.h"

#define cycles 1000000  // Nombre de cycles penser/manger pour chaque philospohe

int N;  // Nombre de philospohes (sera donné par argument plus tard)
// Faire un tableau représentant les fourchettes


sem_t* forks; // Tableau de sémaphores pour les fourchettes

void* philosophe(void* arg) {
    int *id = (int *) arg;
    int left = *id;
    int right = (*id + 1) % N;

    int count = 0;
    while ( count < cycles){
        if (left < right){ // penser
            lock(&forks[left]);
            lock(&forks[right]);
        } else {
            lock(&forks[right]);
            lock(&forks[left]);
        }
        unlock(&forks[left]); // manger
        unlock(&forks[right]);
        count++;
    }
    return (NULL);   
}