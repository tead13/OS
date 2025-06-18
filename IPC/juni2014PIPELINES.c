// Да се напише програма во C програмскиот јазик за броење на зборови во датотека.
// Програмата добива име на влезна датотека како аргумент од командна линија. Процесот чита
// по 128 знаци од влезната датотека, притоа на секое второ читање ги праќа податоците до друг
// процес кој ќе го врши броењето на зборовите (скокнува по едно читање т.е. ги праќа вторите 128
// знаци, па четвртите 128 знаци и т.н.). Како метод на комуникација помеѓу двата процеси да се
// користи концепт на цевки, додека пак за броење на зборовите може да се користи наредбата
// „wc -w“ од Shell.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){

    if(argc < 2){
        fprintf("Nema dovolno adrugmenti od komandna linija!");
        exit(1);
    }

    char *ime_datoteka = argv[1];
    pid_t dete1;
    int pipe1[2]; 
    pipe(pipe1); //kreirame pipe

    if (pipe(pipe1) == -1) {
        perror("pipe1 error");
        exit(1);
    }
    //0-cita
    //1- zapisuva
    FILE *file_open = fopen(ime_datoteka, "r"); //otvarame dat za citanje
    if(file_open == NULL){
        perror("Greska pri otvaranje!");
        exit(1);
    }
    //programa 1 roditel, ke gi isprakja 128znaci sekoj 2 pat
    //programa 2 dete, gi broi zborovite
    dete1 = fork(); //kreirame dete proces

    if(dete1 > 0){ //jas sum vo roditelot - glavniot proces
        close(pipe1[0]); //se zatvora krajot za citanje -ne treba
        dup2(pipe1[1], 1); // go prenasocuvame pipe za pisuvanje
        close(pipe1[1]); //zatvarame bidejkji vekje e prenasocen

        //printf sega odi na pipe
        //scanf si ostanuva kaj so si e od datoteka

        int c; //promenliva znakot sto se cita 
        int flag = 0; //brojac na proctani znaci vo tekovniot blok, pocetok na blokot
        int end = flag+128; //kraj na blokot
        int iteracija = 1; //broi blokovi

        //cita znak po znak od datotekata dodeka ne dojde do end of file
        while(( c = fgetc (file_open)) != EOF){ //FGETC -cita eden znak od datoteka izbrana
            //vo C mi stoi karakterot
            flag++;

            if(iteracija % 2 == 0){ //ako sme vo sekoj 2 blok, pecati karakter
                printf("%c", c);    //izlezot e prenasocen vo pipe
            }

            if(flag+1 == end){ //ako stigne do krajot na blokot
                flag = end;
                end = flag + 128;
                iteracija++;
            }
            
        } 
        fclose(file_open);
        exit(1);   
    }else if(dete1 == 0){ //izvrsuva dete

        close(pipe1[1]);
        dup2(pipe1[0], 0); //otvara pipe za citanje
        close(pipe1[0]);

        //scanf mi e od pipe
        //printf si ide kaj saka

        int c;
        int brojac= 0;
        int in_word = 0; //deka ne sum vo zbor, 0-ne, 1-vo zbor

        //citame od vlez karakter po karakter
        while((c=getchar()) != EOF){ //GETCHAR = GETC -cita od vlez
            if(!isspace(c)){ //ako karakterot ne e prazno mesto
                if(!in_word){   //ako prethodno ne sme bile vo zbor in_word = 0
                    //sega karakterot ne e prazno mesto znaci sme vo zbor
                    //zgolemuvame brojac i vusnost ova se izvrsuva samo na prviot karakter od zborot 
                    brojac++;
                    in_word=1;
                }
            }else{
                in_word = 0; 
            }
        }
        printf("Vkupno zborovi ima %d", brojac);
        
        // execlp("wc", "wc", "-w", NULL);
        // perror("Neuspesno izvresno!");
        exit(1);
    }

    return 0;
}