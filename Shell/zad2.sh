# Да се напише Shell скрипта која што ги наоѓа сите ценовници во тековниот директориум т.е. ги
# бара сите датотеки кои што завршуваат на екстензија .vcf. Доколку има повеќе ценовници, тогаш на
# крајот во првиот ценовник од листата ценовници се додава содржината на другите ценовници и се
# повикува python скриптата со името на првиот ценовник. 


#!/bin/bash

cenovnici=$(ls *.vcf) #vo cenovnici gi lista site dat so .vcf

#proveruva dali cenovnici e prazna, znaci nemalo nitu edna dat .vcf
if [ -z "$cenovnici" ]; then
    echo "Ne postoi nitu edna datoteka"
    exit 1
fi

#go zema prviot cenovnik od listata
#head prikazuva prvi redovi -n1 znaci 1 prv red
#(ako sakam povekje ja menuvam samo br. pr ako sakam 5 reda head -n5)
prv_cenovnik=$(echo "$cenovnici" | head -n1)

#broj na dat, bri kolku redovi ima vo cenovnik 
broj_datoteki=$(echo "$cenovnici" | wc -l)

#ako ima povekje od edna dat
if [ $broj_datoteki -gt 1 ]; then
    
    #gi izminuva site dat i ako dat ne e prvata, se prikazuva sodrzinata vo prvata
    for dat in $cenovnici; do
        if [ "$dat" != "$prv_cenovnik" ]; then
            cat "$datoteka" >> "$prv_cenovnik"
        fi
    done
fi

#se prakja na python zad
python3 zad2.py "$prv_cenovnik"