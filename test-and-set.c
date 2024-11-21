/*  
Deux possibilités après xchgl %eax, (lock) :

1. %eax contient 0 : l’adresse (lock) a été mise de 0 à 1
• Le lock était libre et est maintenant réservé
• Le thread peut entrer en section critique

2. %eax contient 1 : l’adresse (lock) a été mise à 1 mais
valait déjà 1 avant l’appel !
• Le lock n’était pas libre
• On boucle et on ré-essaie autant de fois que nécessaire

• Un seul thread peut rencontrer la situation (1) car
l’opération est atomique
• L’exclusion mutuelle est donc assurée
• Cet algorithme est aussi appelé test-and-set
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct {
    int state;
} lock_t;


int init_lock(lock_t **lock){
    *lock = malloc(sizeof(lock_t));
    (*lock)->state = 0;
}

void lock_lock(lock_t **lock){
    int count = 1;
    while(count == 1){
        __asm__ __volatile__(
            "movl $1, %%ebx\n"
            "xchgl %%ebx, %0\n"
        :"=m" ((*lock)->state),"=b" (count)
        :"m" ((*lock)->state)
        );
    }
}

void unlock_lock(lock_t **lock){
    int *stat = &(*lock)->state;
    __asm__ __volatile__(
        "movl $0, %%ebx\n"
        "xchgl %%ebx, %0\n"
    :"=m" ((*lock)->state),"=b" (*stat)
    :"m" ((*lock)->state)
    );
}

void free_lock(lock_t **lock){
    free(*lock);
}