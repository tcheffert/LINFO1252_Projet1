#include <semaphore.h>
#include <pthread.h>

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

// Fonction simulant un traitement
void process()
{
    for (int i = 0; i < 10000; i++)
        ;
}

// Fonction Reader
void *reader()
{
    // Le(s) lecteur(s) effectue(nt) 2540 lectures
    for (int i = 0; i < N_readings; i++)
    {

        pthread_mutex_lock(&z); // Permet de donner la priorité absolue aux writers

        sem_wait(&db_reader); // Sémaphore permettant aux écrivains de bloquer les lecteurs

        pthread_mutex_lock(&mutex_reader); //? Mutex pour protéger la variable readcount
        readcount++;

        if (readcount == 1)
            sem_wait(&db_writer); // Premier reader réserve la database pour être sûr qu'aucun writer ne l'utilise

        pthread_mutex_unlock(&mutex_reader);
        sem_post(&db_reader);
        pthread_mutex_unlock(&z);

        //===== Section critique =====//
        process(); // On peut process car les writers sont bloqués par le sémaphore db_writer

        pthread_mutex_lock(&mutex_reader);
        readcount--;
        if (readcount == 0)
            sem_post(&db_writer); // dernier reader libère la database écrivain

        pthread_mutex_unlock(&mutex_reader);
    }
}

// Fonction Writer
void *writer()
{
    // Le(s) écrivain(s) effectue(nt) 640 écritures
    for (int i = 0; i < N_writings; i++)
    {

        pthread_mutex_lock(&mutex_writer); // Mutex pour protéger la variable writercount
        writercount++;
        if (writercount == 1)     // arrivée du premier writer
            sem_wait(&db_reader); // bloque les lecteurs
        pthread_mutex_unlock(&mutex_writer);

        sem_wait(&db_writer); // bloque les autres writers et vérifie que la database
                              // n'est pas en train d'être lue

        //===== Section critique =====//
        // Un seul writer à la fois. On peut write car les lecteurs sont bloqués par le sémaphore db_reader et les autres writers par db_writer
        process();

        sem_post(&db_writer); // libère les autres writers

        pthread_mutex_lock(&mutex_writer);
        writercount--;
        if (writercount == 0)     // départ du dernier writer
            sem_post(&db_reader); // libère les lecteurs
        pthread_mutex_unlock(&mutex_writer);
    }
}

int main(int argc, char const *argv[])
{
    pthread_mutex_init(&mutex_reader, NULL);
    pthread_mutex_init(&mutex_writer, NULL);
    pthread_mutex_init(&z, NULL);
    pthread_t writers[2];
    pthread_t readers[2];
    sem_init(&db_reader, 0, 1); // initialisation du semaphore pour writer
    sem_init(&db_writer, 0, 1); // initialisation du semaphore pour reader
    pthread_create(&writers[0], NULL, writer, NULL);
    pthread_create(&writers[1], NULL, writer, NULL);
    pthread_create(&readers[0], NULL, reader, NULL);
    pthread_create(&readers[1], NULL, reader, NULL);
    pthread_join(writers[0], NULL);
    pthread_join(writers[1], NULL);
    pthread_join(readers[0], NULL);
    pthread_join(readers[1], NULL);
    return 0;
}