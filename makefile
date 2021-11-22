is1butf8.o: is1butf8.c
	gcc -c -o is1butf8.o is1butf8.c

isb1of2b.o: isb1of2b.c
	gcc -c -o isb1of2b.o isb1of2b.c

istbutf8.o: istbutf8.c
	gcc -c -o istbutf8.o istbutf8.c

is2butf8.o: is2butf8.c
	gcc -c -o is2butf8.o is2butf8.c 

isb1of3b.o: isb1of3b.c
	gcc -c -o isb1of3b.o isb1of3b.c

is3butf8.o: is3butf8.c
	gcc -c -o is3butf8.o is3butf8.c

isb1of4b.o: isb1of4b.c
	gcc -c -o isb1of4b.o isb1of4b.c

is4butf8.o: is4butf8.c
	gcc -c -o is4butf8.o is4butf8.c

putu.o: putu.c is1butf8.o isb1of2b.o istbutf8.o is2butf8.o isb1of3b.o is3butf8.o isb1of4b.o is4butf8.o
	gcc -c -o putu.o putu.c is1butf8.o isb1of2b.o istbutf8.o is2butf8.o isb1of3b.o is3butf8.o isb1of4b.o is4butf8.o

strlenu8.o: strlenu8.c
	gcc -c -o strlenu8.o strlenu8.c

toloweru.o: toloweru.c
	gcc -c -o toloweru.o toloweru.c is1butf8.o is2butf8.o is3butf8.o is4butf8.o

getu.o: getu.c
	gcc -c -o getu.o getu.c

utox.exe: utox.c getu.o putu.o is1butf8.o isb1of2b.o istbutf8.o is2butf8.o isb1of3b.o is3butf8.o isb1of4b.o is4butf8.o
	gcc -o utox.exe utox.c getu.o putu.o is1butf8.o isb1of2b.o istbutf8.o is2butf8.o isb1of3b.o is3butf8.o isb1of4b.o is4butf8.o
