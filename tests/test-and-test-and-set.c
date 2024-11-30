#include "../headers/TATAS.h"

int N; // Nb threads

void* test(void* arg)
{
    //int *lock = (int*)arg;
    lock_t *lock = (lock_t*)arg;
    for (int i = 0; i < 32768/N ; i++)
    {
        lock_lock(&lock);
        process(); // simulate a process
        unlock_lock(&lock);
    }

    return NULL;
}

void test_init(int N){
    pthread_t *threads = (pthread_t*)malloc(N * sizeof(pthread_t));
    //int *locks = (int*)malloc(N * sizeof(int));
    lock_t *locks = (lock_t*)malloc(N * sizeof(lock_t));

    if (threads == NULL || locks == NULL)
    {
        perror("Error malloc");
        exit(1);
    }
    
    for (int i = 0; i < N; i++)
    {
        if (init_lock(&locks[i]) != 0)
        {
            perror("Error lock init");
            exit(1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&threads[i], NULL, test, &locks[i]) != 0)
        {
            perror("Error thread creation");
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

    for (int i = 0; i < N; i++)
    {
        free_lock(&locks[i]);
    }

    free(threads);
    free(locks);
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <N>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    N = atoi(argv[1]);
    test_init(N);

    return 0;
}