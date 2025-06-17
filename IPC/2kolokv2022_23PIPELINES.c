/*1.Да се напише програма во С програмскиот јазик за pipeline извршување на процеси. На почеток,
програмата дозволува внесување на вредност Х од тастатура, притоа треба да се креираат Х процеси кои
што ќе комуницираат преку pipeline (Х не повеќе од 10). Наредбите се без аргументи и се внесуваат од
тастатура, така што, првата внесена наредба од тастатура е првиот процес, втората наредба е вториот
процес и т.н. Секој нареден процес, комуницира со следниот преку pipeline, така што чита од
претходниот процес, печати во следниот процес (првиот процес – наредба чита од тастатура, додека
последниот процес печати на екран).
2.Бонус: Да се внесе во првиот процес дополнителна вредност Y која, доколку има вредност 0, тогаш
pipeline е стандарден, но, доколку Y е различно од 0, тогаш секоја наредба од pipeline-от, исто така да
печати и на екран и во десниот pipeline (слично на shell наредбата tee).
Пример:
./zadaca
3
ls
cat
wc
Резултатоттреба да ја реализира следнава shell наредба: ls | cat | wc*/

#include <studio.h>
#include <unistd.h>

int main (int argc, char *argv[]){
    int X; 
    scanf("Vnesi go X - broj na procesi: %d", &X);

    // deklarirame pipe ako matrica od 10 procesi so pipe od tipot p[i][2]
    //sekoj proces ke ima fd[0] i fd[1] za citanje i pisuvanje

    //X max e 10, znaci max 10 procesi, 9 pipes
    int p[9][2]; //matrica od pipes, p[i][0] - citanje, p[i][1] - pisuvanje
    n[10][30]; //za site 10 procesi, 30 max znaci za vnes na naredba

    //za sekoj proces ke vneseme naredba koja treba da ja izvrsi 
    for(int i = 0; i < X; i++){
        printf("Vnesi naredba za proces %d.", i+1);
        scanf("%s", n[i]);
    }

    //kreirame pipes, za sekoj par procesi
    for(int i = 0; i < X - 1; i++){
        pipe(p[i]); 
        //kreira pipe i za sekoj pipe preira dete proces, prviot proces e parent
        //i = 0 e prviot proces t.e roditel proces, kreira pipe0 i dete
        //deteto podocna ke stane so i = 1 i ke bide roditel na slednoto dete so id = 2
        //vo ovoj for ciklus decata sozdavaat deca i stanuvaat roditeli itd
        //BITNO; i = 0 e prviot roditel t.e prviot proces, decata se potoa od 1 do 9 
        dete  =  fork(); 

        if(dete > 0) {
            break; //ako e roditel proces, izlez od for ciklusot, negovoto dete prodolzuva
        }
    }

    if(i != 0){ //ako ne e roditel proces
        //site naredbi od 2 do 10, imaat i od 1 do 9 i site citaat od pipe
        close(p[i][1]);//zatvarame pipe za pisuvanje ke otvorime za citanje
        dup2(p[i][0],0); //citaat od kanal na pipe
    }

    if(i != X-1){
        //site procesi osven posledniot pecatt vo pipe
        close(p[i][0]); //zatvora za citanje, otvora za pisuvanje t.e output
        dup2(p[i][1],1);
    }

    execlp(n[i], n[i], NULL);
    //ako execlp() ne uspee
    printf("Neuspesno izvrsuvanje na nareedbata %s", n[i]);
    return 0;
}