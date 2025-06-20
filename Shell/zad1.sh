# Да се напише Shell скрипта која што ќе провери дали во тековниот директориум се наоѓаат датотеките "poraka.txt" и "kontakti.vcf". 
# Доколку ги има, тогаш ги пребарува сите текстуални датотеки во тековниот директориум (со екстензија .txt, игнорирајќи ја датотеката "poraka.txt") 
# и секоја една датотека ја праќа на влез на Python скриптата заедно со "kontakti.vcf". Секоја наредба што ќе се врати, SHELL скриптата треба да ја изврши.

#!/bin/bash

if [[ -f "poraka.txt" && -f "kontakti.vcf" ]]; then
# Проверува дали постојат двете датотеки "poraka.txt" и "kontakti.vcf" во тековниот директориум.

    for file in *.txt; do
    # Почнува циклус кој за секој фајл со екстензија .txt во тековниот директориум ја извршува телото на циклусот.

        if [[ "$file" != "poraka.txt" ]]; then
        # Проверува дали тековниот фајл НЕ е "poraka.txt".

            commands = $(python3 zad1.py "$file" "kontakti.vcf")
            # Ја извршува Python скриптата zad1.py со аргументи:
            # - тековниот .txt фајл
            # - "kontakti.vcf"
            # и ја чува повратната вредност (output) во променливата commands.
            
            for cmd in $commands; do
            # За секоја команда (cmd) од вредноста во променливата commands, изврши ја следната команда:

                $cmd
                # Извршува ја командата cmd како shell команда.

            done
            # Крај на внатрешниот циклус.

        fi
        # Крај на условот за да не се вклучи "poraka.txt".

    done
    # Крај на циклусот за сите .txt фајлови.

else
# Ако условот за постоење на двете датотеки не е исполнет:

    echo "Datotekite 'poraka.txt' ili 'kontakti.vcf' gi nema vo tekovniot direktorium"
    # Испечатува порака дека датотеките ги нема во тековниот директориум.

fi
# Крај на if условот.
