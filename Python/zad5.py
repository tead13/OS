# Да се напише Python скрипта која како аргументи од командна линија добива 2 датотеки (имиња на
# датотеки). Доколку има помалку од 2 датотеки, тогаш треба да се испише порака за грешка и да заврши
# скриптата. Првата датотека содржи парови зборови, по два зборови во секој ред посебно, втората
# датотека е датотека со случаен текст. Паровите зборови на првата датотека се граничници т.е. првиот
# збор е почетната граница а вториот збор е крајот. Скриптата треба да ги најде и отпечати сите зборови
# од втората датотека кои се наоѓаат од првиот граничен збор до вториот и да ги отпечати на екран. Ова
# треба да се направи за сите парови гранични зборови од првата датотека. Доколку некој од граничните
# зборови го нема во втората датотека или првиот не е пред вториот, во тој случај не се печати за тој пар
# гранични зборови ништо.

# Пример:
# Dat1.txt:
# prv vtor
# most koj
# tri kraj
# Dat2.txt
# Sekoj prv den sedime na most pokraj reka, I sekoj vtor od nas e koj od koj od elitata. Na kraj site se vratija na
# rabota

# На екран:
# prv den sedime na most pokraj reka, I sekoj vtor
# most pokraj reka, I sekoj vtor od nas e koj
#JUNSKI ISPIT 2022

import sys

if len(sys.argv) < 3:
    print("nevaliden br na argumenti")
    sys.exit(1)

dat1 = open(sys.argv[1], "r")
dat2 = open(sys.argv[2], "r")

#tekstot od dat2 ke go stavime kako niza od zborovi vo tekst
tekst = dat2.read().split()
pecatiTekst = []


for line in dat1:
    words = line.split()

    if len(words) != 2:
        #ako toj red od dat1 nema 2 granicni zbora prodolzuva so for ciklosot so sledna linija
        continue
    
    g1 = words[0]
    g2 = words[1]

    if g1 in tekst and g2 in tekst:
        #gi barampocetniot i krajniot indeks od nizata na zborovi za da znam od kade do kade da pecatam
        start_index = tekst.index(g1)
        end_index = tekst.index(g2)

        #proveruvm dali startniot zbor e pred krajniot
        if start_index < end_index:
            zaPecatenje = [] #tuka ke gi smestime zborovite koi se za pecatenje
            for i in range(start_index, end_index + 1):
                zaPecatenje.append(tekst[i])
            #niza od zborovi koi treba da se pecatat
            pecatiTekst.append(" ".join(zaPecatenje))
            #vo pecatiTekst gi dodavame site zborovi od zaPEcatenje
            #no gi spojuvame kako eden string so prazni mesta izmedju


for zbor in pecatiTekst:
    print(zbor)
            

