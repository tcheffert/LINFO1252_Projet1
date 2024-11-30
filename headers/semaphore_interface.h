#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <unistd.h>

// Structure du sémaphore
typedef struct
{
    atomic_int value;     // Valeur du sémaphore (atomique pour éviter les conditions de course)
    pthread_mutex_t lock; // Mutex pour protéger les sections critiques
} semaphore_t;

// Initialisation du sémaphore
void semaphore_init(semaphore_t *sem, int initial_value)
{
    sem->value = initial_value;
    if (pthread_mutex_init(&sem->lock, NULL) != 0)
    {
        perror("Erreur d'initialisation du mutex (void semaphore_init)");
        exit(1);
    }
}

// Destruction du sémaphore
void semaphore_destroy(semaphore_t *sem)
{
    if (pthread_mutex_destroy(&sem->lock) != 0)
    {
        perror("Erreur de destruction du mutex (void semaphore_destroy)");
        exit(1);
    }
}

// Fonction Wait (P) avec attente active
void semaphore_wait(semaphore_t *sem)
{
    while (1)
    {
        pthread_mutex_lock(&sem->lock); // Entrée dans la section critique
        if (sem->value > 0)
        {                                     // Si la ressource est disponible
            sem->value--;                     // Consomme une unité de la ressource
            pthread_mutex_unlock(&sem->lock); // Sortie de la section critique
            break;                            // Quitte la boucle
        }
        pthread_mutex_unlock(&sem->lock); // Sortie de la section critique si valeur insuffisante
        // Attente active (busy-wait), ici on peut ajouter un léger délai pour réduire l'utilisation CPU
        sched_yield(); // Cède la CPU à d'autres threads pour minimiser l'impact du busy-wait
    }
}

// Fonction Signal (V)
void semaphore_signal(semaphore_t *sem)
{
    pthread_mutex_lock(&sem->lock);   // Entrée dans la section critique
    sem->value++;                     // Libère une unité de la ressource
    pthread_mutex_unlock(&sem->lock); // Sortie de la section critique
}
