#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *wypisz(void *idWatku) {

    int id;
    id = (long)idWatku;
    printf("Hello SCR! Napisane przez wątek nr %d\n", id);
    pthread_exit(NULL);
}

int main() {

    pthread_t watek[3]; // Utworzenie tablicy wątków

    for(int i = 0; i < 3; i++)
        if(pthread_create(&watek[i], NULL, wypisz, (void *)(long)i))
            printf("\nBłąd tworzenia wątku!\n");

    pthread_exit(NULL);
}
