#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include "TAS.h"


// Fonction simulant un traitement
void process()
{
    for (int i = 0; i < 10000; i++)
        ;
}

int update_counter_with_limit(int *counter, pthread_mutex_t *mutex, int limit)
/* Pre: Update un counter et de façon sécursiée en fonction d'une certaine limite
** Arg: 'counter' un pointeur vers le counter, 'mutex' un pointeur vers le mutex du counter,
**      'limit' un int qui indique la limite du counter
** Post: Renvoit 0 si le counter a bien été update, -1 si la limite a été atteinte */
{
     // Check le compteur global et agit en fonction
    pthread_mutex_lock(mutex);
    if (*counter >= limit)
    { // Dépasse la limite de production ?
        pthread_mutex_unlock(mutex);
        return -1; // Return -1 si on a atteint la limite 
    }
    (*counter)++; // Add 1 au counter 
    pthread_mutex_unlock(mutex); // Débloque le mutex => on évite que plusiers threads touchent au counter
    return 0; // Succès ! Le conteur a bien été changé
}

int update_counter_with_limit_no_mutex(int *counter, int *mutex, int limit)
/* Pre: Update un counter et de façon sécursiée en fonction d'une certaine limite
** Arg: 'counter' un pointeur vers le counter, 'mutex' un pointeur vers le mutex du counter,
**      'limit' un int qui indique la limite du counter
** Post: Renvoit 0 si le counter a bien été update, -1 si la limite a été atteinte */
{
     // Check le compteur global et agit en fonction
    lock_lock(mutex);
    if (*counter >= limit)
    { // Dépasse la limite de production ?
        unlock_lock(mutex);
        return -1; // Return -1 si on a atteint la limite 
    }
    (*counter)++; // Add 1 au counter 
    unlock_lock(mutex); // Débloque le mutex => on évite que plusiers threads touchent au counter
    return 0; // Succès ! Le conteur a bien été changé
}

// philosophers.c

#define cycles 1000000  // Nombre de cycles penser/manger pour chaque philospohe

void problem(int);