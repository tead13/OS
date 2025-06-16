# Да се напише Python скрипта за работа со датотеки. Скриптата, како аргумент од командна линија добива
# име на една влезна датотека. Скриптата треба да ги најде сите телефонски броеви во датотеката и
# сопствениците на тие телефонски броеви. Секој телефонски збор е составен од 9 цифри, притоа
# започнува со 07. Скриптата треба да ги најде сите различни телефонски броеви и на крај да ги отпечати.
# Зборот пред секој телефонски број означува кој е корисникот на кој припаѓа тој телефонски број.
# Скриптата треба да го отпечати на екран името на сопственикот на телефонскиот број и самиот
# телефонски број. Доколку во датотеката истиот телефонски број се појавува со различни корисници,
# тогаш се печати само последниот корисник што ќе биде пронајден во датотеката за тој телефонски број.
# Секој збор од друг збор во датотеката е одделен со празно место.

import sys

if len(sys.argv) != 2:
    print("Nema dovolen broj na argumenti")
    sys.exit(1)

#otvarame dat dobiena kako prv arg vo komandna linija i barame vo nea tel br
dat = open(sys.argv[1], 'r')
phonebook = {} #vo ovoj dict ke gi smestuvame najdenite tel br(key) i nivnite sopstvenici(value)
for line in dat:
    words = line.split()
    for i in range(1, len(words)): 
        word = words[i]
        if word.isdigit() and word.startswith('07') and len(word) == 9: #ova se uslovite da eden zbor e tel br
            sopstvenik = words[i-1]
            broj = word
            phonebook[broj] = sopstvenik

dat.close()

#sega ja imame sostaveno phonebook od datotekata, sleduva da gi pecatime site broevi 
print("Phonebook:")
for broj, sopstvenik in phonebook.items():
    print(f"{sopstvenik}: {broj}")