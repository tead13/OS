# # Да се напише Python скрипта со име „prva.py3“ која како аргументи добива две влезни датотеки. Првата
# датотека е код напишан во Python, додека пак втората датотека е документација која треба да се вметне
# во кодот. Документацијата е организирана така што секоја линија од датотеката најпрво има број, а
# потоа текст. Скриптата треба да ги спои двете датотеки, така што втората датотека (документацијата)
# треба да ја додаде во програмскиот код (првата датотека). Додавањето се прави така што бројката на
# почеток на секоја линија во документацијата кажува на која линија во кодот треба да биде истата
# вметната, а потоа, на крајот на линијата во кодот, се додава линијата од документацијата, започнувајќи
# со #. Новоформираниот код се печати на екран.
# Пример:
# ./prg dat.py3 dok.co
# Dat.py3:
# for i in range(len(X)):
#  for j in range(len(X[0])):
#  result[i][j] = X[i][j] + Y[i][j]
# for r in result:
#  print(r)
# dok.co
# 6 #print the result
# 2 #iterate columns
# 1 #iterate rows
# 3 #sum up
# На екран:
# for i in range(len(X)):#iterate tows
#  for j in range(len(X[0])):#iterate columns
#  result[i][j] = X[i][j] + Y[i][j]#sum up
# for r in result:
#  print(r)#print the result

import sys

if len(sys.argv) < 3:
    print("Nedovolen broj na argumenti")
    sys.exit(1)

dat1 = open(sys.argv[1], "r")
dat2 = open(sys.argv[2], "r")

komentari = {} #recnik, key ke e red od kodot i value ke e komentarot

#ja izminuvame dat2 za da go popolime recnikot
for line in dat2:
    zborovi = line.split()

    if len(zborovi) > 1: #ako ima povekje od 1 el znaci ima red br pa kom
        broj = int(zborovi[0])
        tekst = " ".join(zborovi[1:]) #site zborovi osven prviot gi sostavuva vo 1 string
        komentari[broj] = tekst


#gi cita site linii od kodot i sekoja linija ke bide element od kod_linii
kod_linii = dat1.readlines()

#go pominuvame kodot linija po linija i na sekoj alinija dodavame komm ako ima
for br_line in range(1, len(kod_linii) + 1):
    line = kod_linii[br_line - 1].strip()
    #linija ni e br_line -1 zs u realno vo kod_linii se indeksira od 0 
    #i vsusnost realnata linija 1 vo kod_linii e 0
    #so strip gi trgame site prazni mesta na pocetokot i krajot na elementot

    #proveruvame dali ima komentar za taa linija
    if br_line in komentari:
        line += komentari[br_line]

    print(line) 

dat1.close()
dat2.close()