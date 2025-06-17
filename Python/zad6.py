# Да се напише Python скрипта која како аргументи од командна линија добива имиња на датотеки.
# Скриптата треба да проверува две по две датотеки (секоја датотека со секоја останата датотека) и да
# споредува колку исти Хаштагови има (збор составен со букви и бројки кој започнува со специјалниот
# знак #). Истите Хаштагови и вкупниот број на Хаштагови се запишува во излезна датотека со име
# креирано од спојување на имињата на двете датотеки кои што се споредуваат. На крај, на екран се
# печати кој е најчесто користениот Хаштаг во сите датотеки.

import sys

if len(sys.argv) < 3:
    print("Neovolen broj na argumenti")
    sys.exit(1)

all_hashtags = []
#site datoteki gi stavame vo niza, site osven prviot arg
dats = sys.argv[1:]

#za sekoja dat kreirame niza hashtags koja ke gi sodrzi ht za sekoja dat posebno 
for dat in dats:
    with open(dat, "r") as file: #sekoja dat ja otvarame kako file
        text = file.read() #vo tekst smestuvame sto pisuva vo dat
        words = text.split() #gi delime zbor po zbor
        hashtags = []
        for word in words: #proveruvame zbor po zbor dali e hashtag
            if word.startswith("#"):
                hashtags.append(word)
        all_hashtags.append(hashtags) 
        #vo all_hashtags ke gi dodademe nizi od hashtagovi na sekoja dat posebno

#sporeduvame 2 dat, naodjame zaedniscki #
for i in range(len(all_hashtags)):
    for j in range(i+1, len(all_hashtags)): #i+1 zs ke pocne od taa do nego dat
        file1_hashtags = all_hashtags[i]
        file2_hashtags = all_hashtags[j]

        common_hashtags = []
        for hashtag in file1_hashtags:
            if hashtag in file2_hashtags and hashtag not in common_hashtags:
                common_hashtags.append(hashtag)

        #kreirame dat kade ke gi zapiseme ovie #
        with open("izlez.txt", "a") as output_file:
            output_file.write(f"Istite hashtagovi medju {dats[i]} i {dats[j]}:")
            for hashtag in common_hashtags:
                output_file.write(f"{hashtag}, ")
            output_file.write("\n")

hashtag_count= {} #recnik vo koj ke cuva kolku pati se pojavil nekoj hashtag
#kolku pari se pojavuva hash vo site dat
for hashtags in all_hashtags:
    for hashtag in hashtags:
        if hashtag not in hashtag_count:
            hashtag_count[hashtag] = 1
        else:
            hashtag_count[hashtag] += 1

najcest = None #promenliva koja ke go cuva najcesto koristeniot tag
max_count = 0 #promenliva za cuvaje na max br na pojavuvanja

for hashtag, br in hashtag_count.items():
    if br > max_count:
        max_count = br
        najcest = hashtag

print(f"Najcesto koristen hashtag e {najcest} so {max_count} pojavuvanja")