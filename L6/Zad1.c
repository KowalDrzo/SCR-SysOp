#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


typedef struct stat Stat;


int main() {

    char name[40];
    int i, number;    

    int mmapFd, userFileFd; // FD plików

    char *addr, buffer[1];


    Stat fileStats; // Struktura statystyk pliku
    

    if((mmapFd = open("plik_pamieci", O_RDWR)) < 0) {
        printf("Problem z plikiem\n");
        return 1;
    }
       

    while(1) {

        number = lseek(mmapFd, 0, SEEK_END); // Zmiana położenia deskryptora na koniec pliku + 0B. Zwraca przesunięcie w Bajtach (od początku pliku).

        printf("Nazwa pliku: ");
        scanf("%s", name);

        if((userFileFd = open(name,O_RDONLY)) < 0) {
            printf("Bledny plik!\n");
            continue;
        }
        
        if(fstat(userFileFd, &fileStats) == -1) { // Statystyki / dane pliku wczytywane do fileStats. Zwrócenie -1 oznacza błąd. w tym zadaniu przydatny będzie st_size - rozmiar pliku w B.
            printf("Bledne statystyki!\n");
            continue;
        }  
        
        addr = mmap(NULL, fileStats.st_size+number, PROT_WRITE | PROT_READ , MAP_SHARED, mmapFd, 0);
        // mapowanie. Argumenty: Adres początku mapowania, ilość Bajtów do zmapowania, pozwolenie na odczyt i zapis, flaga pozwalająca mapować dany obszar pamięci przez kilka procesów naraz, deskryptor mapowanego pliku, offset - tutaj niepotrzebny - 0. Funkcja zwraca MAP_FAILED, gdy jej działanie się nie uda.

        if (addr == MAP_FAILED){
            printf("Blad mapowania!\n");
            continue;
        }

        ftruncate(mmapFd, fileStats.st_size + number); // Rozszerzenie rozmiaru pliku


        for(i = 0; read(userFileFd, buffer, 1) > 0; i++) addr[number+i] = buffer[0]; // Wpisywanie danych z buforu do obczaru pamięci powiązanego z plikiem wynikowym.
    }

    return 0;
}
