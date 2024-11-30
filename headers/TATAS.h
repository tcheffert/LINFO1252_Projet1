// TATAS.h
// • Tirons partie du mécanisme de cache :
// • Tant que la variable lock est à 1, le thread TA qui exécute
// sa section critique n’a pas terminé
// • Il n’est pas utile de faire autant d’appels à xchg, qui ont
// pour effet de rallonger la durée de la section critique de TA
// et donc la latence d’accès à leur section critique pour les
// autres threads
// • Une lecture sur la valeur de lock peut se faire dans le cache
// tant que lock est à 1
// • Si un thread lit lock==0 alors il tente d’appeler xchg
// 51
// while (test_and_set(verrou, 1)) {
//  // on a pas obtenu le verrou car on a lu 1
//  // donc on attend de lire 0 pour tenter à nouveau
//  while (verrou) {}
// }
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>

#ifndef TATAS_H
#define TATAS_H

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
    while(*lock){
        asm("Boucle: \n\t"
        
        );
    }

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