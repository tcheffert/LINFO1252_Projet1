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