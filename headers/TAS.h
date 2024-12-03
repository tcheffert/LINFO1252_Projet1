#ifndef TAS_H
#define TAS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>



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