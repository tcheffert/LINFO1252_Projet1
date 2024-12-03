#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "TAS.h"

// Structure du sémaphore
typedef struct
{
    int value;                // Valeur du sémaphore (atomique pour éviter les conditions de course)
    pthread_cond_t condition; // Variable de condition pour réveiller les threads en attente
    int *lock;     // Mutex pour protéger les sections critiques
} semaphore_t;

int checkArg(semaphore_t *sem, const char *func_name)
{
    if (sem == NULL)
    {
        fprintf(stderr, "Erreur dans %s : pointeur de sémaphore NULL !\n", func_name);
        return -1; // Erreur pointeur semaphore: return -1
    }
    return 0; // Pas d'erreurs: return 0
}

// Initialisation du sémaphore
void semaphore_init(semaphore_t *sem, int initial_value)
{
    if (checkArg(sem, "semaphore_init") == -1)
        return; // Erreur argument

    sem->value = initial_value; // Initialisation de la valeur du sémaphore
    // Initialisation du mutex et de la variable de condition
    sem->lock = init_lock(1);
    if (pthread_cond_init(&sem->condition, NULL) != 0)
    {
        perror("Erreur d'initialisation (void semaphore_init)");
        exit(1);
    }
}

// Destruction du sémaphore
void semaphore_destroy(semaphore_t *sem)
{
    if (checkArg(sem, "semaphore_destroy") == -1)
        return; // Erreur argument

    // Destruction du mutex et de la variable de condition si elles ont été correctement initialisées
    // free sem->lock ? 
    free(sem->lock);
    if (pthread_cond_destroy(&sem->condition) != 0)
    {
        perror("Erreur lors destruction (void semaphore_destroy)");
        return;
    }
}

// Fonction Wait (P) avec attente active
void semaphore_wait(semaphore_t *sem)
{
    if (checkArg(sem, "semaphore_wait") == -1)
        return; // Erreur argument

    //pthread_mutex_lock(&sem->lock); // Entrée dans la section critique
    lock_lock(sem->lock);

    while (sem->value <= 0)                             // Tant que la ressource n'est pas disponible
    {                                                   // Les threads en attente sont bloqués sur une condition
        //pthread_cond_wait(&sem->condition, sem->lock); // Attente jusqu'à notification
        // wait 
    }

    sem->value--; // Consomme une unité de ressource

    //pthread_mutex_unlock(&sem->lock); // Sortie de la section critique
    unlock_lock(sem->lock);
}

// Fonction Signal (V)
void semaphore_signal(semaphore_t *sem)
{
    if (checkArg(sem, "semaphore_signal") == -1)
        return; // Erreur argument

    //pthread_mutex_lock(&sem->lock); // Entrée dans la section critique
    lock_lock(sem->lock);

    sem->value++;                         // Augmente le nombre de ressources disponibles
    pthread_cond_signal(&sem->condition); // Réveille un thread en attente

    //pthread_mutex_unlock(&sem->lock); // Sortie de la section critique
    unlock_lock(sem->lock);
}
