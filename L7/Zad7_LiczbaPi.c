#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LWATKOW 16
#define PROBY 999999

/************************************************************************/

void *wylicz(void *idWatku) {

    int id;
    id = (long)idWatku;

    double x, y;
    int licznik = 0;    

    for(int i=0; i < PROBY; i++) {

        x = drand48()*2-1;
        y = drand48()*2-1;

        //printf("\nx: %f, y: %f\n", x, y);

        if(sqrt(x*x+y*y) < 1) licznik++;
    }

    double pi = (licznik*4.0)/PROBY;
    double *wskPi = malloc(sizeof(pi));

    *wskPi = pi;

    printf("Wątek %d uznaje, że liczba pi to: %f\n", id, pi);
    pthread_exit((void*)wskPi);
}

/************************************************************************/

int main() {

    void *wskStatus;
    double piCalkowite = 0.0;
    double *status;

    pthread_t watek[LWATKOW]; // Utworzenie tablicy wątków
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

/************************************************************************/

    for(int i = 0; i < LWATKOW; i++)
        if(pthread_create(&watek[i], &attr, wylicz, (void *)(long)i))
            printf("\nBłąd tworzenia wątku!\n");

/************************************************************************/

    pthread_attr_destroy(&attr); // Usunięcie zawartości &attr
    
    for(int i = 0; i < LWATKOW; i++) {

        pthread_join(watek[i], &wskStatus); // Dołączanie do wątków, by pobrać wartość.
        status = (double*) wskStatus;
        piCalkowite += (*status)/LWATKOW; // Wyliczanie średniej.
    }

/************************************************************************/
    
    printf("\nŚrednia wynosi: %f\n", piCalkowite);
    pthread_exit(NULL);
}
