# Да се напише Python скрипта која како аргумент добива име на датотека во која се запишани 
# ценовник на производи. Во посебен ред од датотеката се наоѓа секој еден од производите, притоа во
# редот најпрво е баркодот на производот, потоа одделен со празно место е името на производот, а потоа
# исто така одделен со празно место е сместена цената на производот. Производите треба да се сместат во
# Хеш структура, каде што клуч ќе е баркодот, а како вредност ќе се чува името на производот. Треба да
# се креира и посебна Хеш структура каде како клуч ќе се земе повторно баркодот на производот, а како
# вредност ќе се чува цената на производот. Потоа, скриптата треба да дозволи да се внесуваат непознат
# број на команди од тастатура. Доколку од тастатура се внесе 1, тогаш низ ценовникот се пребарува по
# име на производ, што значи дека после тоа се внесува името на производот кој се пребарува а на екран
# се печати цената на производот (или дека производот не постои). Доколку се внесе 2, се листаат
# производите чија што цена им е помала од таа внесена од тастатура. Доколку се внесе 3, тогаш се
# печатат сите производи (секој производ во посебен ред) со името и цената на производот. Доколку се
# внесе вредност различна од 1, 2 или 3, тогаш се прекинува со командите и скриптата завршува.
 
import sys

if len(sys.argv) < 2:
    print("Nema dovolen broj na argumenti")
    sys.exit(1)

#prvin od datotekata ke gi kreirame hesh strukturite, kre kreirame 2 dictionaries
products = {} #ke smestime barcod i ime 
prices = {} #ke smestime barkod i cena
dat = open(sys.argv[1], 'r')
for line in dat:
    parts = line.split() #vo sekoj red ima barkod, ime i cena i se podeleni izmedju so prazno mesto, znaci pravime niza za sekoj red i gi kreirame dictionaries
    if len(parts) == 3:
        barcode = parts[0]
        ime = parts[1]
        cena = parts[2]
        products[barcode]=ime
        prices[barcode] = cena
#vaka gi popolnuvame site hesh strukturi
dat.close()

#sega ke vnesuvame komadi od tastatura
print("Izbor na komandi: \n")
print("1 - Prebaraj proizvod po ime\n")
print("2 - Prikazi proizvodi poniski od dadena cena\n ")
print("3 - Prikazi site proizvodi\n")

while True:
    print("Vnesi komanda 1, 2 ili 3 (za kraj vnesi bilo koja druga vrednost):\n")
    command = input()

    if command == '1':
        imeP = input("Vnesi ime na prizvod za prebaruvanje:\n")
        found = False #ke go iskoristime flagot ako ne postoi takov proizvod podocna
        for barcode, ime in products.items():
            if imeP.lower() == ime.lower():
                found = True
                print(f"Proizvod: {ime}, cena: {prices[barcode]} denari\n")
                #vaka so fo ciklus izminuvame dict t.e za sekoj barkod i ime vo products se proveruva da se najde baraniot proizvod
                # koga ke se najde, se pecati negovoto ime i cena, taka sto cenata se zema od dr dict kako ime_na_dict[key] a key ni barkodot 
                #barkodot vekje go imame za daden proizvod koga go izminuvame vo products
        if not found:
            print("proizvodot ne postoi \n")
    elif command == '2':
        cenaVnesena = float(input("Vnesi cena za prebaruvanje:\n"))
        found = False
        for barcode, cena in prices.items():
            if float(cena) < cenaVnesena:
                found = True
                print(f"Proizvod: {products[barcode]}, cena: {cena} denari\n")
        if not found:
            print(f"Nema proizvodi so cena poniska od {cenaVnesena}\n")
    elif command =='3':
        print("Site proizvodi:\n")
        for barcode, ime in products.items():
            print(f"Proizvod: {ime}, cena:{prices[barcode]} denari\n")
    else:
        print("kraj na programata\n")
        break