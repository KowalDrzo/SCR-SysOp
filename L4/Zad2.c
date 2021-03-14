#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int16_t hold = 0;

void a_handler(int signum){

    printf("Do widzenia\n");
    exit(0);
}

/* ---------------------------------------------------------- */

void b_handler(int signum){

    printf("Działam dalej i tak\n");
}

/* ---------------------------------------------------------- */

void c_handler(int signum){

    hold = 1000;
}

/* ---------------------------------------------------------- */

void d_handler(int signum){ return; }

/* ---------------------------------------------------------- */

int main() {
    
    signal(SIGALRM,a_handler);
    signal(SIGTERM,b_handler);
    signal(SIGUSR1,c_handler);
    signal(SIGUSR2,d_handler);

    long i = 0;
    while(1) {
        
        i++;
        usleep(50);
        
        if(hold > 0) {
            
            signal(SIGUSR1,d_handler);
            hold--;
        }
        else signal(SIGUSR1,c_handler);
    }

}
