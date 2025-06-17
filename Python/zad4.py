# Да се напише Python скрипта која како аргумент од командна линија добива име на датотека.
# Скриптата треба да ја измине датотеката и да провери колку различни валути на пари ќе најде. Знаците
# на валутите кои што се пребаруваат се: „$“ „€“ „£“ „kn“ „kr“ „Rp“ „CHF“. Секоја валута започнува со
# знакот на валутата, по кое што следи сумата на истата (одделена со празно место). Доколку после
# валутата нема бројка, тогаш валутата не се зема во предвид. Сите валути што ќе бидат пронајдени во
# датотеката се собираат и се добива вкупна сума по валута. За секоја валута што била пронајдена во
# датотеката, му се дозволува на корисникот да внесе колку денари е истата (не се внесува за оние валути
# кои што не биле пронајдени во тековната датотека). На крајот од изминувањето на датотеката да се
# отпечати вкупната сума (во денари) што била пронајдена во датотеката. Скриптата, на крај треба да
# отпечати кои валути биле пронајдени. 

import sys

if len(sys.argv) < 2:
    print("nevaliden broj na argumenti")
    sys.exit(1)

dat = open(sys.argv[1], "r", encoding="utf-8")

valuti = {"$": 0, "€": 0, "£": 0, "kn": 0, "kr": 0, "Rp": 0, "CHF": 0}

for line in dat:
    words = line.split()
    for i in range(len(words)-1):
        word = words[i] 
        suma = words[i+1]
        if word in valuti and suma.isdigit():
            valuti[word] += int(suma)
dat.close()

vkupno = 0


for val, suma in valuti.items():
    if suma > 0: #znaci bila pronajdena valutata
        kurs = input(f"Vnesi kurs za valuta {val} vo denari\n")
        vkupno += int(suma) * int(kurs)
#taka so for ciklus, za site najdeni valuti ke vnese kurs i ke gi dodae vo vk vo den

#PECATI PRONAJDENI VALUTI SO IZNOS, I NA KRAJ KOLKU VK VO DEN
print("Pronajdeni valuti:\n")
for val, suma in valuti.items():
    if suma > 0:
        print(f"{val}:{suma}")

print(f"Vkupno vo denari: {vkupno}")
