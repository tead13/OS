/*// Да се напише програма во C програмскиот јазик каде како аргументи (од командна линија) добива
// најпрво бројка Х, а потоа листа од команди (без аргументи). Програмата треба да ги изврши во посебни
// процеси секоја една од командите. Доколку бројот на команди е помал од 5, тогаш се извршуваат сериски
// (прво првата, па откако ќе заврши втората се извршува третата и т.н.) притоа првата команда чека Х
// секунди пред да почне да се извршува, останатите не чекаат туку веднаш по претходната почнуваат со
// извршување. Доколку бројот на команди е 5 или повеќе, тогаш сите команди почнуваат паралелно да се
// извршуваат, така што секоја од командите чека Х секунди пред да се изврши. Без разлика како ќе отпочнат
// со извршување командите, излезот од секоја команда да се редиректира во датотека со име исто како и
// бројот на процесот што се извршува командата со наставка .txt (пример: доколку бројот на процесот е 1567,
// тогаш тој процес излезот го редиректира кон датотека со име “1567.txt”). На крај, главниот процес ја печати
// содржината од датотеките што се креирани од процесите на екран. */

#include <stdio.h>   // за printf, fopen
#include <stdlib.h>  // за exit, atoi
#include <unistd.h>  // за fork, execlp, dup2, close, sleep
#include <sys/types.h> // за pid_t
#include <sys/wait.h> // за wait
#include <fcntl.h>  // за open

int main(int argc, char *argv[]){
    if(argc < 3){
        printf("Nema dovolen broj na argumenti");
        return 1;
    }

    //br od komandna e vnesen od tastaura kako string treba da se konvertira vo int
    int X = atoi(argv[1]);
    if(X < 0){
        printf("nevaliden br za X");
        return 1;
    }

    int brKomandi = argc - 2; //gi minusirame imeto na programata i x
    int seriski = brKomandi < 5; //ako e pomalce od 5 seriskike dobie vredn 1

    pid_t dete; //procesot sto ke go kreirame
    pid_t deca[100]; //niza so pids na decata
    int i; 
    char output_dat[30]; //pole za ime na datoteka

    if(seriski){
        //serisko sozdavanje na deca, roditelot gi sozdava edno po edno 
        for(i = 0; i < br_komandi; i++){
            dete = fork();

            if(dete < 0){
                printf("neuspesno preiranje na dete proces");
                return 1;
            }else if(dete == 0){
                break;
            }else{
                waitpid(dete,NULL,0); //ceka da zavrsi deteto
                deca[i] = dete; 
            }
            //ako e dete izleguva od for ciklusot t.e ne sozdava novi deca
            //roditelot ceka da zavrsi ednoto dete za da moze da sozdade novo - seriski 
        }

        if(dete == 0){
            int id = getpid();
            itoa(id, output_dat, 10); //pretvarame id od broj vo string
            //se smestuva vo output_dat, znaci tamu go imame imeto na nasaata dat i samo dodavame .txt
            strcat(output_dat, ".txt");

            int fd = open(output_dat, O_WRONLY, | O_CREAT | O_TRUNC, 0666);
            //otvarame dat za pisuvanje, ako ne postoi ja kreirame, ako postoi ja brise sodrzinata

            dup2(fd, STDOUT_FILENO); //sega stdout e kon datotekata
            close(fd);

            if(i == 0){
                sleep(X);
            }

            execlp(argv[i+2], argv[i+2], NULL); //izvrsuvanje na naredbata
        }
    }else{
        //paralelno sozdavanje na deca
        dete = fork();

        if(dete == 0){
            break; //deteto ne sozdava novi deca, izleguva od for i odi kon izvrsuvanje
        }

        //roditelot rodolzuva da sozdava novi deca
        dete[i] = dete; //se cuva pid i odi ponatamu so kreiranje novi deca
            
        if(dete == 0){
            //sozdavame dat so ime na pid
            int id = getpid();
            itoa(id, output_dat, 10); //pretvarame id od broj vo string
            //se smestuva vo output_dat, znaci tamu go imame imeto na nasaata dat i samo dodavame .txt
            strcat(output_dat, ".txt");

            int fd = open(output_dat, O_WRONLY, | O_CREAT | O_TRUNC, 0666);
            //otvarame dat za pisuvanje, ako ne postoi ja kreirame, ako postoi ja brise sodrzinata

            dup2(fd, STDOUT_FILENO); //sega stdout e kon datotekata
            close(fd);

            sleep(X); //site cekaat 
            execlp(argv[i+2], argv[i+2], NULL);
        }
    }

    //roditelot ceka da zavrsat site nitki za d apotoa ispecati se na ekran
    for(i = 0; i < brNaredbi; i++){
        waitpid(deca[i], NULL, 0);

        itoa(deca[i], output_dat,10);
        strcat(output_dat, ".txt");

        dete = fork();
        //ke kreira novo dete za da ja izvrsi naredbata, da ja prikaze dat
        if(dete == 0){
            execlp("cat", "cat", output_dat, NULL);
        }else{
            waitpid(dete, NULL, 0);
        }
    }

    return 0;
}