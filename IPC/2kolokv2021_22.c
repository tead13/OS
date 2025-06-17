/*Да се напише С програма која работи со процеси и меѓупроцесна комуникација. Главниот процес, како
аргументи од командна линија, добива наредба со аргументи која треба да ја изврши. Потоа, преку
тастатура се внесува колку пати треба да се изврши соодветната наредба Z. Главниот процес креира Z деца
процеси колку што треба да се изврши наредбата, притоа секој еден од деца процесите ја извршува
наредбата еднаш. Иако деца процесите се креираат и се извршуваат паралелно, сепак треба да се овозможи
механизам каде деца процесите ќе се чекаат помеѓу себе т.е. ќе се извршуваат сериски. Главниот процес
треба да брои колку време им требало на деца процесите да завршат со извршување. Комуникацијата
помеѓу деца процесите и синхронизацијата за да се постигне сериско извршување да се изведе со помош на
заедничка (shared) меморија.
Пример:
./prva wc -c -l -d*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int i, Z, id; //i-brojac, Z -broj na deca, id-ID na spodelena memorija
    key_t kluc = 1234; //kluc so koj ke pristapime do shared memory

    int sekundi = 0; //ke broi kolku sec ke rabotat decaprocesite
    int cekaj = 0; //broi kolku deca zavrsile so rabota

    printf("Vnesi ja vrednosta na Z: ");
    scanf("%d", &Z); //kolku deca ke se kreiraat = Z

    //dinamicki ke kreirame niza od deca procesi
    //taa niza sega ke e prazna bidejkji seuste ne se kreirani decata, no koga
    //ke gi kreirame nizata ke se popolni so nivni PID
    pid_t deca[Z];
    char *mem; //pokazuvac kon spodelena memorija

    //kreiranje na memorija so kluc=1234, golemina 1 bajt
    //0664 | IPC_CREAT - owner moze da cita i pisuva, a drugite samo da citaat 
    //i ako vekje ne postoi se kreira nova 
    id = shmget(kluc, 1, 0664 | IPC_CREAT);
    if(id == -1){//so ova proveruvame dali uspesno e kreirana
        printf("Neuspesno dobivanje pristap do memorijata\n");
        return 0;
    }

    //ga prikacuvame memorijata na adresniot kod na procesot, so *mem
    mem = shmat(id, NULL, 0);
    mem[0]=0; //ova ke ni e kako flag koj ke kazuva dali nekoj ja koristi memorijata
    //od 0 do Z, so toa ke znaeme koe dete proces ke raboti so memorijata

    //kreiranje na deca procesi
    for(i = 1; i <= Z; i++){
        deca[i-1] = fork(); //so fork kreirame novo dete proces i pid se smestuva vo nizata deca

        if(deca[i-1] == 0){//ako e dete proces, da izleze od for
            break;
        }
    }//znaci i = 1 ni e dete 1 itn, pogolem od Z e roditel proces

    if(i > Z){//znaci roditelot zavrsil so kreiranje na dete procesi 
        //izvrsuva roditel
        //treba da ceka da zavrsat site deca i tuka ke broi kolku vreme na izvrsuvanje
        while(cekaj < Z){ //cekaj broi kolku deca zavrsile so rabota
            cekaj++;
            mem[0] = cekaj; //koj proces smee da se izvrsuva, koe e sledno na red

            while(waitpid(deca[cekaj-1], 0, WNOHANG) !=  deca[cekaj-1]){
                //uslovot proveruva dali procesot cekaj-1 e zavrsen, cekaj-1 e tekovniot proces sto se izvrsuva
                //ako e zavrsen ke vrati PID i koga ke e ednakov so deca[cekaj-1] znaci deka zavrsil i ke izleze od while
                //ako ne e zavrsen treba da ceka po 1 s i tak ke gi broi sec
                sleep(1);
                sekundi++;
            }
        }
        printf("Na decata im trebaat %d sec da zavrsat\n", sekundi);
    }else{
        //ova ke go izvrsuvaat deca procesite
        //povotorno pritapuvame do memorija
        id = shmget(kluc, 1, 0664);
        if(id == -1){
            printf("neuspesen pristap do memorija\n");
            return 0;
        }
        mem = shmat(id, NULL, 0);
        while(mem[0] != i){
            sleep(1); 
            //dodeka mem[0] ne e = i, procesot ke ceka, dodeka ne dojde na red da se izvrsi
        }

        execvp(argv[1], &argv[1]); //da se izvrsi arg od komandna linija

    }

    return 0;

}