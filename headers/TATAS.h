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

#include "headers/imports.h"

typedef struct {
    int state;
} lock_t;

/**
 * Initialise le verrou.
 * Return -1 si problème d'allocation, et 0 sinon (Succès)
*/
int init_lock(lock_t **lock){
    *lock = malloc(sizeof(lock_t));
    if (*lock == NULL) {
        perror("Problème d'allocation");
        return -1;
    }
    (*lock)->state = 0;
    return 0;
}

void lock_lock(lock_t **lock){
    int count = 1;
    while(count == 1){
        while ((*lock)->state == 1){
            // On attend que le lock soit libre
        };
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