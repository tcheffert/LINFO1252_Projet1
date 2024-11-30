// TAS.h
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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>

#ifndef TAS_H
#define TAS_H

int *locks;

/**
 * Initialise le verrou.
 * Return NULL si problème d'allocation, et 0 sinon (Succès)
*/
int *init_lock(int n){
    int* lock = (int *) calloc(n, sizeof(int));
    if(lock == NULL){
        return NULL;
    }
    return lock;
}

void lock_lock(int *lock){
    asm volatile(
        "1: \n\t"
        "movl $1, %%eax \n\t"
        "xchgl %%eax, %0 \n\t"
        "testl %%eax, %%eax \n\t"
        "jnz 1b \n\t"
    : "+m" (*lock)
    : 
    : "%eax" , "memory"
    );

}

void unlock_lock(int *lock){
    asm volatile(
        "movl $0, %%eax \n\t"
        "xchgl %%eax, %0 \n\t"
    : "+m" (*lock)
    :
    : "%eax" , "memory"
    );
}

void free_lock(){
    free(locks);
}

#endif