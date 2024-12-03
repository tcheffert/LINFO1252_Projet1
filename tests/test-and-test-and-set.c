#include "../headers/TATAS.h"

#include <pthread.h>

int N; // Nb threads

// Fonction simulant un traitement
void process()
{
    for (int i = 0; i < 10000; i++)
        ;
}

void* test(void* arg)
{
    //int *lock = (int*)arg;
    int *lock = (int*)arg;
    for (int i = 0; i < 32768/N ; i++)
    {
        lock_lock(lock);
        process(); // simulate a process
        unlock_lock(lock);
    }

    return NULL;
}

void test_init(int N){
    pthread_t *threads = (pthread_t*)malloc(N * sizeof(pthread_t));

    locks = init_lock(1);

    if (threads == NULL || locks == NULL)
    {
        perror("Error malloc");
        exit(1);
    }
    
    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&threads[i], NULL, test, &locks[0]) != 0)
        {
            perror("Error thread creation, number ");
            exit(1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Error thread join");
            exit(1);
        }
    }

    free(threads);
    free(locks);
}

int main(int argc, char const *argv[])
{
    // valgrind --leak-check=full --show-leak-kinds=all ./tests/test-and-test-and-set 4
    if (argc != 2)
    {
        printf("Usage: %s <N>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    N = atoi(argv[1]);
    test_init(N);

    return 0;
}