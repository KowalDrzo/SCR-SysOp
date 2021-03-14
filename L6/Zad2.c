#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

typedef struct stat Stat;

int main(int argc, char **argv) {

    pid_t pid;
    int myPipeFd[2], fdRet;

    int n;

    pipe(myPipeFd);

    pid = fork();


    char name[40];
    int i;    

    int mmapFd, userFileFd; // FD plików

    char *addr, buffer[1];


    Stat fileStats; // Struktura statystyk pliku
    
    char plik_pamieci[] = "plik_pamieci";

    if((mmapFd = open(plik_pamieci, O_RDWR)) < 0) {
        printf("Problem z plikiem\n");
        return 1;
    }

    if(pid == 0) { 

        execlp("display", "", "-update", "1", "-delay", "2", plik_pamieci, NULL);
    }
       
    else {

        while(1) {

            printf("Nazwa pliku: ");
            scanf(" %s", name);

            if((userFileFd = open(name,O_RDONLY)) < 0) {
                printf("Bledny plik!\n");
                continue;
            }
            
            if(fstat(userFileFd, &fileStats) == -1) { // Statystyki / dane pliku wczytywane do fileStats. Zwrócenie -1 oznacza błąd. w tym zadaniu przydatny będzie st_size - rozmiar pliku w B.
                printf("Bledne statystyki!\n");
                continue;
            }  

            ftruncate(mmapFd, fileStats.st_size); // Rozszerzenie rozmiaru pliku
        
            addr = mmap(NULL, fileStats.st_size, PROT_WRITE | PROT_READ , MAP_SHARED, mmapFd, 0); // mapowanie. Argumenty: Adres początku mapowania, ilość Bajtów do zmapowania, pozwolenie na odczyt i zapis, flaga pozwalająca mapować dany obszar pamięci przez kilka procesów naraz, deskryptor mapowanego pliku, offset - tutaj niepotrzebny - 0. Funkcja zwraca MAP_FAILED, gdy jej działanie się nie uda.

            if (addr == MAP_FAILED){
                printf("Blad mapowania!\n");
                continue;
            }
    
            for(i = 0; read(userFileFd, buffer, 1) > 0; i++) addr[i] = buffer[0]; // Wpisywanie danych z buforu do obczaru pamięci powiązanego z plikiem wynikowym.

            msync(addr, fileStats.st_size, MS_SYNC);
            munmap(addr, fileStats.st_size);
        }


    }

    return 0;
}
