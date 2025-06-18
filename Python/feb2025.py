"""Задача 1: Потребно е да се напише Python скрипта за работа со обработка на наредби. Скриптата добива на влез
(аргумент од командна линија) текстуална датотека во која е сместена содржина на „ls -al” наредбата од некој
директориум. Во секоја линија има податоци за една датотека (посебна линија за посебна датотека). Доколку
датотеката е со екстензија „.c“ тогаш на екран се пишува наредба за компајлирање на датотеката (како извршно име
се сместува името на датотеката без екстензијата) а потоа (во нов ред) се печати наредба за извршување на
програмата. Доколку датотеката е со екстензија „.sh“ и доколку истата не е со привилегии за извршување, тогаш прво
се печати наредба за поставување на привилегии за извршување а потоа се извршува истата. Ова се прави за секоја
датотека (линија) во текстуалната датотека. Во продолжение е пример за компајлирање, поставување на привилегии
за извршување и повикување на програмите/скриптите
Primer terminal:

goranj@goranj-VirtualBox:~/example_folder$ ls -al
total 44
drwxrwxr-x 2 goranj goranj 4096 Feb 10 12:12 .
drwxr-x--- 19 goranj goranj 4096 Feb 10 12:11 ..
-rw-rw-r-- 1 goranj goranj 874 Feb 10 12:12 nitki1.c
-rw-rw-r-- 1 goranj goranj 311 Feb 10 12:12 proces_primerl.c 
-rwxrwxr-x 1 goranj goranj 100 Feb 10 12:12 shell1.sh_new
-rw-rw-r-- 1 goranj goranj 170 Feb 10 12:12 shell2.sh
-rw-rw-r-- 1 goranj goranj 128 Feb 10 12:12 shell3.sh
-rwxrwxr-x 1 goranj goranj 72 Feb 10 12:12 shell4.sh
-rw-rw-r-- 1 goranj goranj 82 Feb 10 12:12 shell5.sh
-rwxrwxr-x 1 goranj goranj 80 Feb 10 12:12 shell6.sh   
-rw-rw-r-- 1 goranj goranj 1438 Feb 10 12:12 tekst.txt

goranj@goranj-VirtualBox:~/example_folder$


На екран:
gcc nitki1.c -o nitki1
./nitki1
gcc proces_primer1.c -o proces_primer1
./proces_primer1
chmod +x shell2.sh
./shell2.sh
chmod +x shell3.sh
./shell3.sh
./shell4.sh
chmod +x shell5.sh
./shell5.sh
./shell6.sh
    
"""

import sys

broj_argumenti= len(sys.argv)-1

if broj_argumenti==1:
    datoteka=open(sys.argv[1],"r")
    next(datoteka) #ja skoka prvata linija
    for line in datoteka:
        zborovi=line.split() #gi delit liniite po prazni mesta
        if len(zborovi) < 9:  # Ako ima premalku zborovi vo redot, prodolzi
            continue
        #jas imav vaka ama za posigurno so proverka
        #ime=zborovi[8].split(".")#go delime imetttttttto so tocka

        ime=zborovi[8]
        dozvoli=zborovi[0]
        if "." in ime:
            delovi=ime.split(".")
            if len(delovi)<2:
                continue #ako nema barem 2 dela skokni ja linijata
            
            ekstenzija=delovi[-1] #mozit i 1 ama se zematttt -1 deka -1 stalno go zemat posledniot del
            samoime=delovi[0]

            if ekstenzija == "c":#ako ekstenzijata e c
                print(f"gcc {ime} -o {samoime}")#ne treba \n poso print avtomatski dodava
                print(f"./{samoime}")
            elif ekstenzija  == "sh": #ako e  ekstenzijata sh
            #dozvoli=zborovi[0].split() , ako beshe vaka samo po prazni mesta kje gi podelese
                #dozvoli=list(zborovi[0])  #vaka ke se podelit po karakteri
                #nemat potreba od ova psoo in provervit bukva po bukva(znaci ova nadolu e opcionalen podolg nacin)
                #imadozvola=0
                #for dozvola in dozvoli:
                   # if dozvola == "x":#ako immmma dozvola
                     #   imadozvola=1
                #if(imadozvola):
                    #print(f"./{samoime}")#ima dozvola
                #else:
                    #print(f"chmod +x {ime}")#nema dozvola
                    #print(f"./{ime}")
                if "x" in dozvoli:
                    print(f"./{ime}")
                else:
                    print(f"chmod +x {ime}")
                    print(f"./{ime}")
    datoteka.close()

else:
    print(f"Nema dovolen broj argumenti") 
    quit()