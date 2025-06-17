// Да се напише програма во C програмскиот јазик каде ќе се овозможи внесување на име на датотека како
// аргумент на програмата. Потоа, програмата треба да изврши мемориско мапирање на датотеката во
// адресниот простор. Датотеката е составена од N цели броеви. Главниот процес, треба да ја измине
// датотеката и да провери колку цели броеви има во (мемориски мапираната) датотека. Потоа, треба да
// креира 10 нитки, каде што секоја една од нитките ќе генерира случаен број од -10 до 2500, и ќе провери
// дали го има во датотеката. Доколку го има, бројот што е случајно генериран го сместува во низа, приота
// доколку некоја нитка повеќе пати го генерира истиот број, тогаш треба да се знае колку пати е секој
// еден број пронајден (генериран). Секоја нитка генерира 20 случајни броеви и потоа завршува. Откако ќе
// завршат сите нитки, главниот процес ги пренесува најдените броеви на дете процес, кое што тој го
// креира. Дете процесот е задолжено да отпечати на екран кои броеви биле пронајдени и колку пати во
// влезната датотека. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

//definiram konstanti
#define NUM_THREADS 10
#define NUM_RANDOMS 20
#define MIN_RANDOM -10
#define MAZ_RANDOM 2500
#define MAX_NUMBERS 20*NUM_THREADS

//globalni promenlivi 
int *mapped_data; //pokazuvac kon pocetok memoriski mapiranite podatoci 
int num_Numbers; //kolku celi broevi ke ima vo datotekata
int found_numbers[MAX_NUMBERS]; //broevite koi ke gi najde vo datotekata
int found_count[MAX_NUMBERS]; //kolku pati se najdeni broevite

//funkcija sto gi izvrsuva nitkite
void *prebaraj(void *t) {
    int id = *((int *)t); //id na nitka
    int found_inThread[NUM_RANDOMS]; //pronajdeni br vo nitkata

    //generirame slucajni broevi i barame dali postojat vo datotekata
    //i pocnuva od id*num_randoms bidejkji ako e nitka 0 ke pocne od 0 ke odi do 20, ako e 1 ke odi od 1*20 do 40 itn
    for(int i = id * NUM_RANDOMS; i < id * NUM_RANDOMS + NUM_RANDOMS; i++) {
        int random_num = rand() % (MAX_RANDOM - MIN_RANDOM + 1) + MIN_RANDOM;
        for(int j = 0; j < num_Numbers; j++) {
            if(mapped_data[j] == random_num) { //ako ne najden vo dat
                found_inThread[i] = random_num;
                break;
            }//tuka moze da se se spoi so count 

        }
    }
    //pravime update na glavnata lista na pronajdeni broevi
    for(int i = 0; i < NUM_RANDOMS; i++) {
        if(found_inThread[i] != 0) {
            int found = 0;

            for(int j = 0; j < MAX_NUMBERS; j++) { //barame dali vekje postoi vo  nizata
                if (found_numbers[j] == found_inThread[i]) { //ako postoi ke zgolemime vo found_count
                    found_count[j]++;
                    found = 1;
                    break;
                }-
            }
            if (!found) { //ako ne sme go nasle go dodavame
                for (int j = 0; j < MAX_NUMBERS; j++) {
                    if (found_numbers[j] == 0) { 
                        found_numbers[j] = found_inThread[i];
                        found_count[j] = 1;
                        break;
                    }
                }
            }
        }
    }
    pthread_exit(NULL);
}

//broi kolku celi broevi ima vo datotekata
int countNumbers(const char *filename) {
    FILE *file = fopen(filename, "r"); 
    if (file == NULL) {
        printf("Failed to open file\n");
        return -1; 
    }

    int count = 0;
    int num;

    while (fscanf(file, "%d", &num) == 1) {
        count++;
    }

    fclose(file);
    return count; 
}

int main(int argc, char *argv[]) {

    num_Numbers = countNumbers(argv[1]);

    printf("Number of elements in the file: %d\n", num_Numbers);

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return 1;
    }

    int file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("lseek failed");
        close(fd);
        return -1;
    }
    lseek(fd, 0, SEEK_SET);

    mapped_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(mapped_data == MAP_FAILED) {
        perror("Memory mapping failed");
        close(fd);
        return 1;
    }

    close(fd);

    pthread_t nitki[NUM_THREADS];
    int t[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        t[i] = i;
        pthread_create(&nitki[i], NULL, prebaraj, (void*)&t[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(nitki[i], NULL);
    }

    pid_t dete = fork();

    if (dete == 0) {
        for (int i = 0; i < MAX_NUMBERS; i++) {
            if (found_numbers[i] != 0) {
                printf("Number: %d, Count: %d\n", found_numbers[i], found_count[i]);
            }
        }
        return 0;
    } else if (dete > 0){
        wait(NULL);
    } else {
        perror("Fork failed");
        return 1;
    }

    return 0;
}