#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

// Fonction simulant un traitement
void process()
{
    for (int i = 0; i < 10000; i++)
        ;
}

int update_counter_with_limit(int *counter, pthread_mutex_t *mutex, int limit)
/* Pre: Reçoit une pointer vers un counter et l'update de façon sécursiée
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
    return 0; // Indicates success
}