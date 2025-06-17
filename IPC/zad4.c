// Да се напишат две програми во C кои ќе работат со процеси, нитки и споделена (shared)
// меморија. Двете програми комуницираат помеѓу себе со споделена (заедничка) меморија со
// клуч 12345. Втората програма, внесува од тастатура низа од 1000 броеви и истите ги запишува
// во заедничката меморија, а потоа, со помош на функцијата rand() (од библиотеката stdlib.h)
// случајно избира една од нитките на првата програма да го пребара својот број. Една нитка не
// може да се избере повеќе пати, така што треба да се грижи втората програма на следно бирање
// да избере некоја од преостанатите нитки.

// Првата програма, дозволува да се внесат Х броеви од тастатура (Х исто така се внесува од
// тастатура и може да е со вредност најмногу 10). Потоа, програмата креира Х нитки, така што
// секоја нитка ќе го пребарува соодветниот број внесен од тастатура. Секоја нитка, чека да и
// дојде редот (да биде избрана од втората програма) за да го пребара бројот во низата од 1000
// броеви. На секоја секунда проверува дали она е на ред да пребарува, и притоа доколку е
// избрана, пребарува низ низата од 1000 броеви и печати на екран колку пати е пронајден
// соодветниот број. Откако ќе заврши, сигнализира на втората програма дека може да избере
// следна нитка за извршување.
// PROGRAMA 1

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 12345
#define MAX_NUMBERS 10
#define ARRAY_SIZE 1000

typedef struct {
    int numbers[ARRAY_SIZE];
    int current_thread;
    int visited[MAX_NUMBERS];
} sharedMemory;

int numbersSearch[MAX_NUMBERS];
int t[MAX_NUMBERS];

void *prebaraj(void *arg) {
    int id = *(int *)arg;
    
    while(1) {
        sleep(1);

        int shmid = shmget(SHM_KEY, 1024, 0666);
        if(shmid < 0) {
            perror("ERROR");
            exit(1);
        }

        sharedMemory *shm = shmat(shmid, NULL, 0);
        if (shm == (void *)-1) {
            perror("ERROR");
            exit(1);
        }

        if(shm->current_thread == id) {
            int counter = 0;
            for(int i = 0; i < ARRAY_SIZE; i++) {
                if(shm->numbers[i] == numbersSearch[id]) {
                    counter++;
                }
            }
            printf("Nitka %d: Brojot %d e pronajden %d pati.\n", id, numbers_to_search[id], count);
            
            shm->visited[id] = 1;
            shm->current_thread = -1;

        }

        shmdt(shm);
        break;

    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int X;
    printf("Vnesete broj (max 10): ");
    scanf("%d", &X);

    if(X > MAX_NUMBERS) {
        printf("Dozvoleni se maksimum 10 broevi");
        return 1;
    }

    for(int i = 0; i < X; i++) {
        printf("Vnesete broj: ");
        scanf("%d", &numbersSearch[i]);
    }

    int shmid = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT);
    if(shmid < 0) {
        perror("ERROR");
        exit(1);
    }

    sharedMemory *shm = shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("ERROR");
        exit(1);
    }

    for(int i = 0; i < MAX_NUMBERS; i++) {
        shm->visited[i] = 0;
    }

    pthread_t nitki[MAX_NUMBERS];
    for(int i = 0; i < X; i++) {
        t[i] = i;
        pthread_create(&nitki[i], NULL, prebaraj, (void *)&t[i])
    }

    for (int i = 0; i < X; i++) {
        pthread_join(threads[i], NULL);
    }

    shmdt(shm);
    
    return 0;
}

// PROGRAMA 2

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>

#define SHM_KEY 12345
#define ARRAY_SIZE 1000

typedef struct {
    int numbers[ARRAY_SIZE];
    int current_thread;
    int visited[10];
} sharedMemory;

int main(int argc, char *argv[]) {

    int shmid = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT);
    if(shmid < 0) {
        perror("ERROR");
        exit(1);
    }

    sharedMemory *shm = shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("ERROR");
        exit(1);
    }

    printf("Vnesete %d broevi:\n", ARRAY_SIZE);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        scanf("%d", &shm->numbers[i]);
    }

    srand(time(NULL));
    int threads_used[10] = {0};
    int threads_remaining = 10;

    while (threads_remaining > 0) {
        int tId = rand() % 10;
        if(!threads_used[tId]) {
            threads_used[tId] = 1;
            threads_remaining--;

            shm->current_thread = tId;

            while(!shm->visited[tId]) {
                sleep(1);
            }
        }
    }

    shmdt(shm);

    return 0;

}
