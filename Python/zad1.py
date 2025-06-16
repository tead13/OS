# Да се напише Python скрипта која како аргументи добива имиња на две датотеки. 
# Во првата датотека е сместен текст кој треба да се обработи, додека пак, во втората датотека е сместен именик со контакти. 
# Скриптата треба да ја измине првата датотека и да ги најде сите е-маил адреси (формат: kime@firma.com) и да креира команда 
# спремна за испраќање на маил со конкретна порака. Командата се формира во следниов формат:
# mail -s "poraka" kime@firma.com -u Ime Prezime

# За секоја емаил адреса што ќе се пронајде во влезната датотека, се креира ваква наредба, каде на местото од kime@firma.com се сместува 
# конкретната најдена емаил адреса. Во втората датотека, чие што име се добива како влез на скриптата, е сместен именик со познати емаил адреси, 
# така што во секој ред е сместена посебна емаил адреса, заедно со името и презимето на корисникот на емаил адресата (одделени со празно место). 
# Скриптата треба да ја провери секоја пронајдената емаил адреса од првата датотека дали ја има во именикот креиран од втората датотека. 
# Доколку пронајдената емаил адреса ја има во втората датотека, тогаш Ime и Prezime се заменуваат со името и презимето 
# пронајдени во именикот за таа емаил адреса. Инаку, се игнорираат од наредбата. Параметарот "poraka" е текстуална 
# порака чиј што текст е сместен во датотека со име "poraka.txt". Доколку емаил адресата ја има во именикот, тогаш 
# пред текстот од пораката се додава следниот текст: "Pocituvan Ime Prezime,", каде име и презиме се земаат исти од претходно. 
# Доколку емаил адресата ја нема во именикот, тогаш "poraka" останува непроменета. 
# Секоја ваква формирана наредба се печати на екран, притоа не се печати повеќе пати ако се пронајде истата емаил адреса повеќе пати.

import sys

if len(sys.argv) < 3:
    print("Nema dovolen broj na argumenti")
    sys.exit(1)

#najprvo ke ja otvorime dat1 i ke ja obrabotime t.e ke gi barame site mail adresi koi se vo dat1
dat1 = open(sys.argv[1], 'r')
emails = [] #prazna niza vo koja ke gi smsestime site pronajdeni mail adr

#ke ja izmineme dat prvin red po red, a potoa zbor po zbor
for line in dat1:
    words = line.split() #sekoj red go delime na zborovi  !funkcijata split() gi dlei zborovite spored prazni mesta
    for word in words:
        if '@' in word and '.' in word:
            parts = word.split('@') #idejata e ako zborot go podelime na delov spored @ i e ednakov na 2, togas e mail adresa
            if len(parts) == 2 and '.' in parts[1]:
                emails.append(word)

dat1.close()
#vaka ja obrabotivme dat1 i site mail adresi gi stavivme vo niza email
#sega ke provverime dali tie mail adresi gi ima vo imenikot od dat2

dat2 = open(sys.argv[2], 'r')
imenik = [] #tuka ke gi smestime kontaktite za baranite email adresi 

#ja izminuvame dat2 i da gi najdeme adresite koi se poklopuvaat
for line in dat2:
    words2 = line.split()
    if len(words2) == 3: #znaci imame 3 elementi vo nizata, email, ime i prezime
        email = words2[0]
        ime = words2[1]
        prezime = words2[2]
        if email in emails:
            imenik.append((email, ime, prezime)) #ako go ima mailot vo dat2 pravime eden kontakt od email, ime, prezime i dodavame vo imenik

dat2.close()
#so ovaa ja izminavme dat2 i gi smestivme site kontaktite vo imenik
#sega ke ja otvorime datoteka poraka.txt i ke ja procitame porakata

poraka_dat = open("zad1_poraka.txt", 'r')
poraka = poraka_dat.read() #vo promenlivata poraka go smestuva tekstot od datotekata poraka.txt
poraka_dat.close()


#posleden del e da se se napravat komandite
for email in emails:
    found = False #ova ke ni e znmence, ako mailot go ima vo kontakti ke se smeni porakata ako ne samo ke se ispecati komandata
    for contact in imenik:
        if contact[0] == email: #kontakt[0] e emailot, kontakt[1] e ime, kontakt[2] e prezime a site 3 se eden element od imenik
            found = True
            ime = contact[1]
            prezime = contact[2] #niv ke gi vmetneme vo komandata
            #kreirame porana na mailot
            message = f"Pocituvan {ime} {prezime}, \n {poraka}"
            #na kraj ke ja ispecatime komandata full izmeneta
            print(f'mail -s \"{message}\" {email} -u {ime} {prezime}')
            break #ako go najdeme kontaktot, nema potreba da barame ponatamu, samo edna komanda ke ispecatime za eden mail
    if not found:
        #ako mailot ne go najdeme vo imenikot, porakata ne se menuva i komandata ke se smeni samo mailot
        print(f'mail -s \"{poraka}\" {email} -u Ime Prezime')

#kraj, potoa odi da se povika vo shell 

