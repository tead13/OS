/*2. Да се напише С програма за работа со процеси и редирекција на податоците. Програмата
иницијално треба да го реализира извршувањето на shell наредбата напишана подолу со помош
на C програми. Вториот и третиот процес, пред да почнат да се извршуваат дозволуваат
внесување на цел број од тастатура, кој, доколку е 0, тогаш не се менува ништо во
извршувањето и се извршува наредбата како што треба, но, доколку е различно од 0, тогаш
податоците од левиот pipe се печатат на екран, а процесот податоците ги добива од тастатура.
Основна верзија
cat dat.txt | tail 9 | wc -c
Сценарио ако и кај вториот и кај третиот процес се внесе од тастатура 0
cat dat.txt | tail 9 | wc -c
Сценарио ако кај вториот се внесе различно од 0 а кај третиот 0
cat dat.txt
tail 9 | wc -c
Сценарио ако кај вториот се внесе 0 а кај третиот различно од 0
cat dat.txt | tail 9
wc -c
Сценарио ако и кај вториот и кај третиот се внесе различно од 0
cat dat.txt
tail 9
wc -c*/

#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t dete1, dete2; //inicijalizirame 2 dete procesa
    int p1[2], p2[2]; //kreirame 2 pipe
    int t1, t2; //vrednosti sto ke gi vnese korisnikot pri sekoe izvrsuvanje na naredba 2 i 3

    printf("Vnesi vrednost za t1 (0 za da ne se menuva nisto): ");
    scanf("%d", &t1); 

    if(t1 == 0){//ne menuvame nisto kreirame pipe1
        pipe(p1);
    }

    dete1 = fork(); //kreirame prvo dete proces za izvrsuvanje na naredba2 so tail

    if(dete1 == 0){
        //izvrsuvanje na naredba2 
        if(t1 == 0){
            close (p1[1]); //zatvarame za pisvanje
            dup2(p1[0], 0); //vlez t.e za citanje na naredba2
        }

        printf("Vnesi vrednost za t2 (0 za da ne se menuva nisto): ");
        scanf("%d", &t2);

        if(t2 == 0){// znaci ne menuvam enisto i kreirame pipe2
            pipe(p2);
        }

        dete2 = fork(); //kreirame dete2 za izvrsuvanje na naredba3
        if(dete2 == 0){
            //ivrsuvanje na naredba3
            if(t2 == 0){
                close(p2[1]); //ja zatvora pisuvanje, bidejkji nemu mu treba za citanje
                dup2(p2[0], 0);
            }
            execlp("wc", "wc", "-c", NULL); //izvrsuvanje na naredba3
        }else{
            //izvrsuvanje na naredba2
            if(t2 == 0){
                close(p2[0]); //zatvora za citanje, bidejkji treba da napise
                dup2(p2[1],1); //ko koristi pipe2 da go zapise izlezot na naredbata
            }
            execlp("tail", "tail","9", NULL); //izvrsuvanje na naredba2)
        }


    }else{
        //izvrsuva roditel t.e naredba1 = cat dat.txt
        if(t1 == 0){
            close(p1[0]); //na naredba1 ne mu treba pipe za citanje tuku za pisuvanje
            dup2(p1[1], 1); //p1[1] stanuva izlez na anredba1
        }

        execlp("cat", "cat", "dat.txt", NULL) //izvrsuvanje na naredba1
    }
    return 0;
}