#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
  
int main(int argc, char **argv) {

    int fdFifo, fdFile; // File decryptor potoku i pliku
  
    char *myfifo = "mojpotok";
    int n;
  
    mkfifo(myfifo, 0666); // Tworzenie potoku
  
    char buffer[50];

    fdFifo = open(myfifo, O_WRONLY); // Otwarcie potoku w trybie do zapisu

    for(int i = 1; i < argc; i++) {

        printf("Otwieram plik %s\n", argv[i]);

        fdFile = open(argv[i], O_RDONLY);
        
        while((n = read(fdFile, &buffer, 50)) > 0) write(fdFifo, &buffer, n); // Odczytanie i wysłanie danych

        close(fdFile);
    }
    close(fdFifo); // Zamknięcie potoku

    return 0;
}
