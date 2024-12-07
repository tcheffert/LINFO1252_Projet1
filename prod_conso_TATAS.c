#include "headers/semaphore_interface_TATAS.h"
#include "headers/imports_TATAS.h"
#include "headers/TATAS.h"

#include <pthread.h>

// Initialisation
#define N 8            // places dans le buffer
#define N_elems 131072 // nombre d’éléments produits (et donc consommé)

int total_produced = 0;
int total_consumed = 0;
int *count_mutex; // Pour protéger les counts (éviter que plusieurs threads modif les counts)

int *mutex;
semaphore_t empty;
semaphore_t full;

int buffer[N];
int idxNextItem_P = 0; // Index du prochain item à produire
int idxNextItem_C = 0; // Index du prochain item à consommer

// Insert un item dans le buffer
void insert_item(int item)
{
    buffer[idxNextItem_P] = item; // Place l'item dans le buffer
    idxNextItem_P++;              // On update l'index

    // On check si le new idx sort du buffer
    if (idxNextItem_P >= N)
        idxNextItem_P = 0; // Si >= taille du buffer, on revient au début
}

int remove_item()
{
    int item = buffer[idxNextItem_C]; // Get l'item
    idxNextItem_C++;                  // Va au prochain
    if (idxNextItem_C >= N)
        idxNextItem_C = 0; // Si >= taille du buffer, on revient au début
    return item;
}

// Producteur
// Signature changée pour ne pas avoir de warnings plus tard avec pthread_create
void *producer(void *arg)
{
    while (1)
    {
        int update = update_counter_with_limit_no_mutex(&total_produced, count_mutex, N_elems);
        if (update == -1)
            break; // On a atteint la limite de production!

        // Simule une production en dehors de la zone critique
        process();

        semaphore_wait(&empty);           // attente d’une place libre
        lock_lock(mutex); // Mutual exclusion

        // Section critique

        insert_item(total_produced); // On utilise total_prod pour savoir lequel on place dans le buffer

        unlock_lock(mutex);
        semaphore_signal(&full); // il y a une place remplie en plus
    }
    pthread_exit(0);
}

// Consommateur
// Signature changée pour ne pas avoir de warnings plus tard avec pthread_create
void *consumer(void *arg)
{
    while (1)
    {
        int update = update_counter_with_limit_no_mutex(&total_consumed, count_mutex, N_elems);
        if (update == -1)
            break; // On a atteint la limite de consommation!

        semaphore_wait(&full); // attente d’une place remplie
        lock_lock(mutex);

        // Section critique

        remove_item();
        unlock_lock(mutex);
        semaphore_signal(&empty); // il y a une place libre en plus

        // Consomme en dehors de la zone critique
        process();
    }
    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    int num_producers, num_consumers;

    // ./program <num_producers> <num_consumers>
    if (argc != 3)
    {
        printf("Usage: %s <num_producers> <num_consumers>\n", argv[0]);
        return -1;
    }
    num_producers = atoi(argv[1]);
    num_consumers = atoi(argv[2]);

    // Check si les valeurs données par argument sont positives et donc valides
    if (num_producers <= 0 || num_consumers <= 0)
    {
        printf("Erreur : Le nombre de producteurs (%d) et le nombre de consommateurs (%d) doit être positif!\n", num_producers, num_consumers);
        return -1;
    }

    // Initialise les semaphores
    semaphore_init(&empty, N); // N places libres au début
    semaphore_init(&full, 0);  // 0 places full au début
    // Initialise les mutexes
    mutex = init_lock(1);      // Initialise mutex pour la section critique dans producer/consumer
    count_mutex = init_lock(1); // Initialise le mutex qui protège les counters

    // Créé les threads producteurs et consommateurs
    pthread_t producers[num_producers], consumers[num_consumers];

    for (int i = 0; i < num_producers; i++)
    {
        pthread_create(&producers[i], NULL, producer, NULL);
        // printf("Producer thread %d created\n", i);  // Debug message
    }
    for (int i = 0; i < num_consumers; i++)
    {
        pthread_create(&consumers[i], NULL, consumer, NULL);
        // printf("Consumer thread %d created\n", i);  // Debug message
    }

    // On attend que tous les threads finissent
    for (int i = 0; i < num_producers; i++)
    {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < num_consumers; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    // Clean les mutexes et semaphores
    free(mutex);
    free(count_mutex);
    semaphore_destroy(&empty);
    semaphore_destroy(&full);

    return 0;
}
