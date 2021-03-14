#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

int main(int argc, char **argv) {

    if(argc != 2) {
        printf("Błędna ilość argumentów!\n");
        return 1;
    }

    pid_t pid;
    
    int myPipeFd[2];
    int pipeState = pipe(myPipeFd);     // Utworzenie potoku

    char buffer[50];
    char buffer2[50];

    int fileDes = open(argv[1], O_RDONLY);

    pid = fork();   // Uruchomienie podprocesu
 
    if(pid == 0) { // Proces dziecka      
        
        close(myPipeFd[1]);

        while(read(myPipeFd[0], buffer2, sizeof(buffer2)) > 0) printf("#%s#", buffer2);
    }

    else { // Proces rodzica

        close(myPipeFd[0]);    

        while(read(fileDes, buffer, sizeof(buffer)) > 0) write(myPipeFd[1], buffer, sizeof(buffer));
        
        close(myPipeFd[1]);
    }

    return 0;
}
