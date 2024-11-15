#include "imports.h"
// Initialisation
#define N 8            // places dans le buffer
#define N_elems 131072 // nombre d’éléments produits (et donc consommé)

int total_produced = 0;
int total_consumed = 0;
pthread_mutex_t count_mutex; // Pour protéger les counts (éviter que plusieurs threads modif les counts)

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

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

int update_counter(int *count)
/* Pre: Reçoit une pointer vers un counter et l'update de façon sécursiée
** Post: Renvoit 0 si le counter a bien été update, -1 si la limite a été atteinte */
{
    // Check le compteur global et agit en fonction
    pthread_mutex_lock(&count_mutex);
    if (*count >= N_elems)
    { // Dépasse la limite de production ?
        pthread_mutex_unlock(&count_mutex);
        return -1; // Return -1 si on a atteint la limite de prod/conso
    }
    (*count)++;                         // Add 1 au counter (prod ou conso)
    pthread_mutex_unlock(&count_mutex); // Débloque le mutex => on évite que plusiers threads touchent au counter
    return 0;
}

// Producteur
//Signature changée pour ne pas avoir de warnings plus tard avec pthread_create
void* producer(void*)
{
    while (1)
    {
        int update = update_counter(&total_produced);
        if (update == -1)
            break; // On a atteint la limite de production!

        // Simule une production en dehors de la zone critique
        process();

        // item = produce(item);
        sem_wait(&empty);           // attente d’une place libre
        pthread_mutex_lock(&mutex); // Mutual exclusion

        // Section critique

        insert_item(total_produced); // On utilise total_prod pour savoir lequel on place dans le buffer

        pthread_mutex_unlock(&mutex);
        sem_post(&full); // il y a une place remplie en plus
    }
    pthread_exit(0);
}

// Consommateur
//Signature changée pour ne pas avoir de warnings plus tard avec pthread_create
void* consumer(void*)
{
    while (1)
    {
        int update = update_counter(&total_consumed);
        if (update == -1)
            break; // On a atteint la limite de consommation!

        sem_wait(&full); // attente d’une place remplie
        pthread_mutex_lock(&mutex);

        // Section critique

        int item = remove_item();
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // il y a une place libre en plus

        // Consomme en dehors de la zone critique
        process();
    }
    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    int num_producers, num_consumers;

    // Command-line arguments: ./program <num_producers> <num_consumers>
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
    sem_init(&empty, 0, N); // N places libres au début
    sem_init(&full, 0, 0);  // 0 places full au début
    // Initialise les mutexes
    pthread_mutex_init(&mutex, NULL);       // Initialise mutex pour la section critique dans producer/consumer
    pthread_mutex_init(&count_mutex, NULL); // Initialise le mutex qui protège les counters

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
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&count_mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
