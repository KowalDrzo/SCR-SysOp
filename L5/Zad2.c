#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 

int main(int argc, char **argv) {

    pid_t pid;
    int myPipeFd[2], fdRet;

    int n;
    
    char buffer[50];

    pipe(myPipeFd);

    pid = fork();

    if(pid == 0) { // Proces dziecka

        close(myPipeFd[1]);
        close(0);
        dup(myPipeFd[0]);
        close(myPipeFd[0]);

        sleep(2);
        execlp("display", "display", "-", NULL); // Uruchomienie display'a
    }
    else { // Proces rodzica

        close(myPipeFd[0]);
        
        fdRet = open(argv[1], O_RDONLY);
        
        while((n = read(fdRet, &buffer, 50)) > 0) write(myPipeFd[1], &buffer, n);

        close(fdRet);
    }
    
    return 0;
}
