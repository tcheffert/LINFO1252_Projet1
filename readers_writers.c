#include "imports.h"

// Nombre de fois où les lecteurs et écrivains s'effecturont
#define N_readings 2540
#define N_writings 640

// Initialisation des mutex et des semaphores
pthread_mutex_t mutex_reader;
pthread_mutex_t mutex_writer;
pthread_mutex_t z;

sem_t db_reader;
sem_t db_writer;

int writercount = 0;
int readcount = 0; // nombre de readers

// Fonction Reader
void *reader()
{
    // Le(s) lecteur(s) effectue(nt) 2540 lectures
    for (int i = 0; i < N_readings; i++)
    {

        pthread_mutex_lock(&z); // Priorité absolue aux writers

        sem_wait(&db_reader); // Sémaphore permettant aux écrivains de bloquer les lecteurs

        pthread_mutex_lock(&mutex_reader); // Protège la variable readcount
        readcount++;

        if (readcount == 1)
            sem_wait(&db_writer); // Premier reader réserve la database pour être sûr qu'aucun writer ne l'utilise

        pthread_mutex_unlock(&mutex_reader);
        sem_post(&db_reader);
        pthread_mutex_unlock(&z);

        //===== Section critique : lecture simulée =====//
        process(); // On peut process car les writers sont bloqués par le sémaphore db_writer

        pthread_mutex_lock(&mutex_reader);
        readcount--;
        if (readcount == 0)
            sem_post(&db_writer); // Last reader libère la database écrivain

        pthread_mutex_unlock(&mutex_reader);
    }
    pthread_exit(0);
}

// Fonction Writer
void *writer()
{
    // Le(s) écrivain(s) effectue(nt) 640 écritures
    for (int i = 0; i < N_writings; i++)
    {

        pthread_mutex_lock(&mutex_writer); // Protège la variable writercount
        writercount++;
        if (writercount == 1)     // Arrivée du premier writer
            sem_wait(&db_reader); // Bloque les lecteurs
        pthread_mutex_unlock(&mutex_writer);

        sem_wait(&db_writer); // Bloque les autres writers et vérifie que la database n'est pas en train d'être lue

        //===== Section critique: écriture simulée =====//
        // printf("Writer %ld is writing. Writercount: %d\n", pthread_self(), writercount);
        // Un seul writer à la fois. On peut write car les lecteurs sont bloqués par le sémaphore db_reader et les autres writers par db_writer
        process();

        sem_post(&db_writer); // libère les autres writers

        pthread_mutex_lock(&mutex_writer);
        writercount--;
        if (writercount == 0)     // départ du dernier writer
            sem_post(&db_reader); // libère les lecteurs
        pthread_mutex_unlock(&mutex_writer);

        // printf("Writer %ld finished writing.\n", pthread_self());
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

    // Check si les valeurs données par argument sont positives et donc valides
    if (nb_readers <= 0 || nb_writers <= 0)
    {
        printf("Erreur : Le nombre de lecteurs (%d) et le nombre d'écrivains (%d) doit être positif.\n", nb_readers, nb_writers);
        return -1;
    }

    pthread_mutex_init(&mutex_reader, NULL);
    pthread_mutex_init(&mutex_writer, NULL);
    pthread_mutex_init(&z, NULL);
    sem_init(&db_reader, 0, 1);
    sem_init(&db_writer, 0, 1);

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

    pthread_mutex_destroy(&mutex_reader);
    pthread_mutex_destroy(&mutex_writer);
    pthread_mutex_destroy(&z);
    sem_destroy(&db_reader);
    sem_destroy(&db_writer);

    return 0;
}