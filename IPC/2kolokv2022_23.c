/*1.Да се напишат две програми во С кои ќе работат со IPC. Од првата програма истовремено може да се
креира само еден процес, додека пак од втората програма истовремено (паралелно) може да се
креираат најмногу 200 процеси. Првата програма со процесите од втората програма комуницираат преку
заедничка меморија (shared memory) преку клучот 1234. Првата програма ја креира меморијата (од 1КВ)
и ја прикачува во сопствениот адресен простор. Доколку првиот процес не успее да ја креира и прикачи
меморијата во сопствениот адресен простор, тогаш пробува тоа да го направи со мапирана меморија (со
датотека “dat.txt”). Доколку и мапираната меморија не успее да ја прикачи во сопсвениот адресен
простор, печати порака за грешка и завршува. Доколку успее или со заедничка меморија или со
мапирана, во меморијата запишува наредба (без аргументи, која ја добива како аргумент од командна
линија), и завршува.
2.Втората програма, без разлика колку паралелни процеси се креирани од истата, секој процес
проверува дали може да ја прикачи заедничката меморија од првата програма (со истиот клуч) во
сопствениот адресен простор, и притоа доколку не успее пробува со мапирана меморија (со истата
датотека). Доколку не успее на двата начини да прикачи меморија во адресниот простор, прекинува со
извршување. Доколку успее да прикачи меморија на еден од двата начини, чека наредбата да биде
запишана во меморијата. Доколку не е запишана наредба, на секои 10 секунди проверува пак дали има
запишано наредба, притоа вкупно чека 1 минута (до 6 пати). Во моментот кога ќе биде запишана
наредбата во меморијата, се извршува и завршува. Доколку после 1 минута (6 пати по 10 секунди) се
уште не е запишана наредбата, а притоа има прикачено заедничка меморија, следно истото го пробува и
со мапирана меморија. Ако и во мапираната меморија нема запишано наредба, печати порака за грешка
и завршува.
Пример:
./prv ls
./vtor*/

#include <stdio.h>     // за printf, NULL
#include <stdlib.h>    // за exit, malloc, free
#include <string.h>    // за memcpy, strcmp, strcpy
#include <sys/types.h> // за key_t, pid_t
#include <sys/ipc.h>   // за IPC_CREAT, shmget, shmat
#include <sys/shm.h>   // за shmget, shmat, shmdt
#include <sys/mman.h>  // за mmap, munmap, MAP_SHARED
#include <sys/stat.h>  // за режима на датотека
#include <fcntl.h>     // за O_RDWR, O_CREAT
#include <unistd.h>    // за ftruncate, execlp, close, sleep
#include <sys/wait.h>  // за waitpid, wait

int main(int argc, char *argv[]){
    key_t kluc = 1234; 
    char *dat = "dat.txt"; //ime na dat sto treba da se mapira vo slucaj da ne uspee shmget
    int k = 0; //flag so koj ke oznacuvame dali rabotime so shmget ili mmap

    int id, fd; //id za shm, fd za dat za mmap
    char *mem; //pokazumac koj shm 

    //probuvame da kreirame spodelena memorija so kluc 1234
    id = shmget(kluc, 1024, 0664 |IPC_CREAT);
    if(id != -1){
        //ako e uspesno kreirana, togas go stavame pokazuvacot mem na shm
        mem = shmat(id, NULL, 0);
    }

    //ako ne e uspesno kreirana shm, kreirame mmap
    if(id == -1 || mem == (void *)-1){
        //ako shmat ne uspee, mem vrakja vrednost (void *)-1, zat e toj uslov
        printf("Neuspesno prikacuvanje na shared memory, probuvame so mmap\n");

        //kreirme mapirana memorija so dat.txt
        fd = open(dat, O_RDWR | O_CREAT, 0666);

        if(fd == -1){
            printf("Neuspesno otvoranje na datotekata\n");
            return 0; //kraj na programata
        }

        //treba da ja prosirime dat do 1kb
        if(ftruncate(fd, 1024) == -1){
            printf("Neuspresno prosiruvanje na datotekara\n");
            return 0; //kra na programata
        }

        //uspesno kreiranje na mmap, ja mapirame dat
        mem = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
        if(mem == MAP_FAILED){
            printf("Neuspesno mapiranje na memorijata");
            return 0; //kraj na programata
        }
        k = 2; //flag za mmap, ako k = 2 znaci deka rabotime so mmap
    }else{
        k = 1; //flag za shm, ako k = 1 znaci deka rabotime so shm
    }//memoriite se kreirani 

    //zapisuvanje na naredba vo memoriite:
    if(k == 1){
        mem[0] = '1'; //ova ni e flag deka ima naredba
        strcpy(mem + 1, argv[1]); //vo memoorijata ja zapisuvame naredbata
        shmdt(mem); //se odjavuvame od shm
    }else if(k == 2){
        mem[0] = '1'; //ova ni e flag deka ima naredba
        strcpy(mem + 1, argv[1]); //vo memoorijata ja zapisuvame naredbata
        munmap(mem, 1024); //se odjavuvame od mmap
    }
    return 0; //kraj na proggrama1
}

//programa2.c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main(int argc, char *argv[]){
    key_t kluc = 1234; 
    char *dat = "dat.txt";
    int k = 0; //1 - shm, 2 - mmap
    int fd, id; //fd za mmap, id za shm
    char *mem; //pokazuvac za mem

    //da probame prvo da se priklucime na shm, ako ne uspee togas na mmap
    id = shmget(kluc, 1024, 0664);
    if(id != -1){
        mem = shmat(id, NULL, 0);
    }

    if(id == -1 || mem = (void *)-1){
        printf("Neuspesno povrzuvanje so shared memory, ke probame so mmap");

        fd = open(dat, O_RDWR);
        if(fd == -1){
            printf("Neuspesno prikacuvanje na mapirana memorija");
            return 0; //kraj na programata
        }

        mem = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if(mem == MAP_FAILED){
            printf("Neuspesno mapiranje na memorijata");
            close(fd);
            return 0; //kraj na programata
        }
        k = 2;
    }else{
        k = 1;
    }

    //proveruva dali ima naredba vo mem, i brojac do 6 pati 
    //for ciklus za cekanje 6*10sec
    for (int i = 0; i < 6; i++){
        if(mem[0] == '1'){ //ako ima naredba
            break; //kraj na for ciklusot za cekanje 
        }
        sleep(10);
    }
    //ako i = 6, znaci naredbata ne e procitana
    //ako i < 6, znaci naredbata e procitana i treba da se izvrsi 
    if(i < 6){
        printf("Izvrsuvam naredba %s.", mem + 1);
        execlp(mem + 1, mem  +1, NULL);
        if(k == 1){
            shmdt(mem);
        }else if(k == 2){
            munmap(mem, 1024);
            close(fd);
        }
    }else{
        printf("Naredbata ne e procitana po 60sec, kraj ");
    }
    return 0; //kraj na programata2
}
