/*Да се напишат две програми во C кои ќе работат со процеси, нитки и споделена (shared)
меморија. Двете програми комуницираат помеѓу себе со споделена (заедничка) меморија со
клуч 12345. Втората програма, внесува од тастатура низа од 1000 броеви и истите ги запишува
во заедничката меморија, а потоа, со помош на функцијата rand() (од библиотеката stdlib.h)
случајно избира една од нитките на првата програма да го пребара својот број. Една нитка не
може да се избере повеќе пати, така што треба да се грижи втората програма на следно бирање
да избере некоја од преостанатите нитки.
Првата програма, дозволува да се внесат Х броеви од тастатура (Х исто така се внесува од
тастатура и може да е со вредност најмногу 10). Потоа, програмата креира Х нитки, така што
секоја нитка ќе го пребарува соодветниот број внесен од тастатура. Секоја нитка, чека да и
дојде редот (да биде избрана од втората програма) за да го пребара бројот во низата од 1000
броеви. На секоја секунда проверува дали она е на ред да пребарува, и притоа доколку е
избрана, пребарува низ низата од 1000 броеви и печати на екран колку пати е пронајден
соодветниот број. Откако ќе заврши, сигнализира на втората програма дека може да избере
следна нитка за извршување.*/

//programa1
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define kluc 12345
#define MAX_NUM 10
#define ARRAY_SIZE 1000

//prvin ke ja definirame zaednickata memorija
typedef struct{
    int broevi[ARRAY_SIZE];
    int current_thread;
    int izvrseni_nitki[MAX_NUM]; //0 ako ne e, 1 ako e
}sharedMemory;

//niza od broevi koi treba da se prebaruvaat i id na nitkite
int barajBR[MAX_NUM];
int t[MAX_NUM];


//funkcija sto ke ja izvrsuva sekoja nitka
void * prebaraj(void* arg){
    int id = *(int*)arg;

    //sekoja sec proveruva dali e na red
    while(1){
        sleep(1);

        //se povrzuva so memorijata
        int shmid = shmget(kluc, sizeof(sharedMemory), 0666);
        if(shmid < 0){
            printf("neuspesno shmget");
            return 1;
        }

        sharedMemory *shm = shmat(shmid, NULL, 0);
        ifif(shm == (void*)-1){
            printf("neuspesno shmat");
            return 1;
        }

        if(shm->current_thread == id){
            int counter = 0;
            for(int i = 0; i < ARRAY_SIZE; i++){
                if(barajBR[id] == shm->broevi[i]){
                    counter++;
                }
            }
            printf("Nitka %d: broj %d e pronajden %d pati.\n", id, barajBR[id], counter);

            shm->izvrseni_nitki[id] = 1;
            shm->current_thread = -1; //signalzira deka moze programa2 da izbere nova nitka

            shmdt(shm);
            break;
        }
        pthread_exit(NULL);
        //ili moze samo return NULL;
    }

}


int main(int argc, char *argv[]){
    int X; //broj na nitki 
    scanf("Vnesi broj na nitki: %d", &X); 

    if(X > MAX_NUM){
        printf("Dozvoleni se maximum 10");
        return 1;
    }

    //vnesuvanje na br sto treba sekoja nitka da go prebaruva
    //nitka so id0 ke prebaruva broj na poz niza[0]
    for(int i = 0; i < X; i++){
        scanf("Vnesi broj: %d", barajBR[i]);        
    }

    //kreirame shm
    int shmid = shmget(kluc, sizeof(sharedMemory), 0666 | IPC_CREAT);
    if(shmid < 0){
        printf("neuspesno kreiranje na memorija");
        return 1;
    }

    //se povrzuvame za memorijata
    sharedMemory *shm = shmat(shmid, NULL, 0);
    if(shm == (void*)-1){
        printf("neuspesno prikacuvanje na memorija");
        return 1;
    }

    //go inicijalizirame poleto za izvrseni nitki 
    //postavuvame deka nitu edna do sega ne bila izvrsena
    //isto pravime i za koja nitka momentaln treba da se izvrsi deka e -1, nitu edna
    for( int i =0; i< MAX_NUM; i++){
        shm->izvrseni_nitki[i] = 0;
    }
    shm->current_thread = -1;

    pthread_t nitki[X];
    for(int i = 0; i< X; i++){
        t[i] = i;
        pthread_create(&nitki[i], NULL, prebaraj, (void*)&t[i]);
    }

    //ceka da zavrsat site
    for(int i = 0; i< X; i++){
        pthread_join(nitki[i], NULL);
    }
    shmdt(shm);

    return 0;

}


//programa2
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>

#define kluc 12345
#define ARRAY_SIZE 1000
#define MAX_NUM 10

typedef struct{
    int broevi[ARRAY_SIZE];
    int current_thread;
    int izvrseni_nitki[MAX_NUM]; //0 ako ne e, 1 ako e
}sharedMemory;

int main(int argc, char *argv[]){
    //kreiranje i povrzuvanje so memorija
    int shmid = shmget(kluc, sizeof(sharedMemory), 0666 | IPC_CREAT);
    if(shmid < 0){
        printf("neuspesno kreiranje na memorija");
        return 1;
    }

    //se povrzuvame za memorijata
    sharedMemory *shm = shmat(shmid, NULL, 0);
    if(shm == (void*)-1){
        printf("neuspesno prikacuvanje na memorija");
        return 1;
    }

    //vnesuva borevi vo nizatai igi zapisuva vo shm
    printf("Vnesi %d broevi"\n, ARRAY_SIZE);
    for(int i = 0; i < ARRAY_SIZE; i++){
        scanf("%d", &shm->broevi[i]);
    }

    //inicijalizirame generatotr na slucajni br so rekovno vreme 
    srand(time(NULL));

    int izbrani_nitki[MAX_NUM] = {0}; //niza vo koja ke se belezi koi nitki se iskoristeni
    //so {0} avtomatski site gi stavame na 0, deka ne se izbereni 
    int threads_remaining = 10; //brojac, uste kolku ostanale

    while(threads_remaining > 0){
        //se dodeka ne se izbrani site nitki 
        int t_id = rand() % 10;

        if(!izbrani_nitki[t_id]){
            //ako nitkata ne e vo izbranite 
            izbrani_nitki[t_id] = 1;
            thread_remaining--;

            shm->current_thread = t_id;

            //ceka dodeka nitkata ne se pojavi vo izvrseni nitki 
            while(!shm->izvrseni_nitki[t_id]){
                sleep(1);
            }
        }
    }
    shmdt(shm);

    return 0;
}