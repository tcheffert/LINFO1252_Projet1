#include "headers/semaphore_interface_TATAS.h"
#include "headers/imports_TATAS.h"
#include "headers/TATAS.h"

// Nombre de fois où les lecteurs et écrivains s'effecturont
#define N_readings 2540
#define N_writings 640

// Initialisation des mutex et des semaphores
int *mutex_reader;
int *mutex_writer;
int *z;

int *totalR_mut;
int *totalW_mut;

semaphore_t db_reader;
semaphore_t db_writer;

int totalR = 0; // Nombre de reader jusqu'à présent
int totalW = 0; // Nombre de write jusqu'à présent
int writercount = 0;
int readcount = 0; // nombre de readers

// Fonction Reader
void *reader()
{
    // Le(s) lecteur(s) effectue(nt) 2540 lectures
    while (1)
    {
        int update = update_counter_with_limit_no_mutex(&totalR, totalR_mut, N_readings);
        if (update == -1)
            break; // On a atteint la limite de readings!

        lock_lock(z); // Priorité absolue aux writers

        semaphore_wait(&db_reader); // Sémaphore permettant aux écrivains de bloquer les lecteurs

        lock_lock(mutex_reader); // Protège la variable readcount
        readcount++;

        if (readcount == 1)
            semaphore_wait(&db_writer); // Premier reader réserve la database pour être sûr qu'aucun writer ne l'utilise

        unlock_lock(mutex_reader);
        semaphore_signal(&db_reader);
        unlock_lock(z);

        //===== Section critique : lecture simulée =====//
        process(); // On peut process car les writers sont bloqués par le sémaphore db_writer

        lock_lock(mutex_reader);
        readcount--;
        if (readcount == 0)
            semaphore_signal(&db_writer); // Last reader libère la database écrivain

        unlock_lock(mutex_reader);
    }
    pthread_exit(0);
}

// Fonction Writer
void *writer()
{
    // Le(s) écrivain(s) effectue(nt) 640 écritures
    while (1)
    {
        int update = update_counter_with_limit_no_mutex(&totalW, totalW_mut, N_writings);
        if (update == -1)
            break; // On a atteint la limite de readings!

        lock_lock(mutex_writer); // Protège la variable writercount
        writercount++;
        if (writercount == 1)     // Arrivée du premier writer
            semaphore_wait(&db_reader); // Bloque les lecteurs
        unlock_lock(mutex_writer);

        semaphore_wait(&db_writer); // Bloque les autres writers et vérifie que la database n'est pas en train d'être lue

        //===== Section critique: écriture simulée =====//
        // printf("Writer %ld is writing. Writercount: %d\n", pthread_self(), writercount);
        // Un seul writer à la fois. On peut write car les lecteurs sont bloqués par le sémaphore db_reader et les autres writers par db_writer
        process();

        semaphore_signal(&db_writer); // libère les autres writers

        lock_lock(mutex_writer);
        writercount--;
        if (writercount == 0)     // départ du dernier writer
            semaphore_signal(&db_reader); // libère les lecteurs
        unlock_lock(mutex_writer);
    }
    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <nombre_lecteurs> <nombre_ecrivains>\n", argv[0]);
        return -1;
    }

    int nb_readers = atoi(argv[1]);
    int nb_writers = atoi(argv[2]);

    // Check if the values given by argument are positive and valid
    if (nb_readers <= 0 || nb_writers <= 0)
    {
        printf("Erreur : Le nombre de lecteurs (%d) et le nombre d'écrivains (%d) doit être positif.\n", nb_readers, nb_writers);
        return -1;
    }

    mutex_reader = init_lock(1);
    mutex_writer = init_lock(1);
    z = init_lock(1);
    totalR_mut = init_lock(1);
    totalW_mut = init_lock(1);

    semaphore_init(&db_reader, 1);
    semaphore_init(&db_writer, 1);

    pthread_t writers[nb_writers];
    pthread_t readers[nb_readers];

    for (int i = 0; i < nb_readers; i++)
    {
        pthread_create(&readers[i], NULL, reader, NULL);
    }
    for (int i = 0; i < nb_writers; i++)
    {
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    for (int i = 0; i < nb_readers; i++)
    {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < nb_writers; i++)
    {
        pthread_join(writers[i], NULL);
    }

    free(mutex_reader);
    free(mutex_writer);
    free(z);
    free(totalR_mut);
    free(totalW_mut);
    semaphore_destroy(&db_reader);
    semaphore_destroy(&db_writer);

    return 0;
}
