/* Да се напише програма во C која како аргумент добива име на непостоечка датотека.
Програмата најпрво креира дете процес. Родител процесот треба да генерира 1000 случајни
броеви со помош на функцијата rand() (од библиотеката stdlib.h) и истите ги запишува на екран
(кои со помош на редирекција се запишуваат во датотеката) и родител процесот завршува. Дете
процесот проверува дали може да ја отвори датотеката, и ако не може заспива 1 секунда. Потоа
проверува пак, и заспива пак 1 секунда, се додека не успее да ја отвори датотеката. Чим успее
да ја отвори, чита 1000 броеви од тастатура (редиректирани од датотеката) и ги сместува во
низа. Потоа, дете процесот, со помош на 4 нитки (секоја нитка пребарува по 250 броеви), го
наоѓа најголемиот број и го печати на екран. */

#include <stdio.h>   // за printf, fopen, fclose, fscanf, fprintf
#include <stdlib.h>  // за exit, rand, NULL, malloc, free
#include <unistd.h>  // за fork, sleep, pid_t
#include <pthread.h> // за нишки (pthread_create, pthread_join)
#include <fcntl.h>   // за flags при отворање на датотеки (не мора, но e тука)

#define NUMBERS 1000 //kolku broevi treba da se geneiraat
#define THREADS 4 //kolku nitki

int broevi[NUMBERS]; //tuka deteto ke gi smesti br sto ke gi procita od dat
int max_values[THREADS]; //tuka ke se smestat max br sto ke gi najde sekoja nitka

//funkcija sto ja izvrsuva nitkata, t.e bara max broj 
void* find_max(void* arg){
    int id = *(int*)arg; //go zema id na nitkata od gl programa
    max_values[id] = broevi[id*250]; 
    //go zema prviot broj da e max od site ponatamu sto ke gi proveruva
    //vaka postaavuvame koja nitka koi broevi ke gi prebaruva, od koj br ke pocne
    //pr id = 0, pocuva od 0*250=0, id = 1 povnuva od 250 itn

    for( int i = id * 250; i < id*250+250; i++){
        if(broevi[i] > max_value[id]){
            max_value[id] = broevi[i];
        }
    }

    return NULL; //nitkata zavrsuva
}

//glavna programa
int main(int argc, char *argv[]){
    //proverka za argumenti mora
    if(argc != 2){
        printf("nema dovolem broj na argumenti")
        return 1;
    }

    char *fileName = argv[1];
    pid_t dete = fork(); 

    if( dete < 0 ){
        perror("FORK FAILED");
        return 1;
    }

    if (dete == 0){
        //dete proces izvrsuva
        FILE* file = NULL;
        
        //probuva da ja otvori datoteka ako ne moze ceka 1sec
        while((file = fopen(fileName, "r")) == NULL){
            sleep(1);
        }

        for(int i = 0; i < NUMBERS; i++){
            fscanf(file, "%d", &broevi[i]);
            //brojot sto go cita od dat, go smestuva vo nizata
        }
        fclose(file);

        //kreirame nitki 
        pthread_t nitki[THREADS]; //gi deklarirame nitkite
        int t[THREADS]; //tuka ke se cuvaat id na nitkite

        for (int i = 0; i < THREADS; i++){
            t[i] = i; //id na nitkata
            pthread_create(&nitki[i], NULL, find_max, &t[i]);
        }

        //cekame site nitki da zavrsat za da se izvrsi glavnata
        for(int i = 0; i < THREADS; i++){
            pthread_join(&nitka[i], NULL);
        }

        //koga ke zavrsat site nitki, se izvrsuva glavnata
        //go barame najgolemiot broj 
        int max = max_values[0];
        for(int i = 1; i < THREADS; i++){
            if(max_values[i] > max){
                max = max_values[i];
            }
        }
        printf("Najgolemiot broj e %d", max);


    }else{
        //roditel proces izvrsuva
        FILE* file = fopen(fileName, "w"); //ja otvara dat za pisuvanje
        if(file == NULL){
            perror("greska pri otvaranje na dat");
            return 1;
        }
       
        for(int i = 0, i < NUMBERS; i++){
            int random_number = rand();
            fprint(file,"%d\n", random_number); //vo dat
            printf("%d, ", random_number); //na ekran
        }
        fclose(file);
    }
    return 0;
}