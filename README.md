# DVGB01-lab3
Detta är en README file som beskriver koden till en virituell minnes simulator.
Det är ett program som körs på spagetti-kod men som verkar fungera!
Hela programet körs på en file och kräver ej någon MAKE file och man inte så själv vill dela upp den i flera filer.

Programet simulerar ett virituellt minne på 64KB med en ramstorlek på 256 bytes, fysika minnet består av ramar x 256 bytes.
Först väljer man hur många ramer som används, hur stårt det fysiska minnet är. Får som misnt vara 1 men kan vara lika många som får plats i en int.
Sendan väljer man en av tre algoritmer:
FIFO - first in frist out, första sidan som kommer in i de fysiksa minnet åker ut.
LRU - least recently used, den sidan som det var längst sedan den används åker ut.
Optimal - den sidan som det kommer ta längst tid innan den kommer tillbaka åker ut.
Sist väljer man en file som innehåller de "virituella minnet".


Programet består av vmsim.c och vmsim.h

För att köra programet behöver man länka den med math.h.

Compilering:
gcc vmsim.c -o vmsim -lm

Körning:
./vmsim -n -a -f

-n: antal ramar, får ej vara 0
-a: FIFO || LRU || Optimal
-f: lab3/trace_1.dat || lab3/trace_2.dat
