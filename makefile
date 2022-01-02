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

isb1ofu8.o: isb1ofu8.c
	gcc -c -o isb1ofu8.o isb1ofu8.c

putu.o: putu.c is1butf8.o isb1of2b.o istbutf8.o is2butf8.o isb1of3b.o is3butf8.o isb1of4b.o is4butf8.o
	gcc -c -o putu.o putu.c is1butf8.o isb1of2b.o istbutf8.o is2butf8.o isb1of3b.o is3butf8.o isb1of4b.o is4butf8.o

strlenu8.o: strlenu8.c
	gcc -c -o strlenu8.o strlenu8.c

toloweru.o: toloweru.c
	gcc -c -o toloweru.o toloweru.c is1butf8.o is2butf8.o is3butf8.o is4butf8.o

fgetu.o: fgetu.c
	gcc -c -o fgetu.o fgetu.c

getu.o: getu.c
	gcc -c -o getu.o getu.c

fputu.o: fputu.c
	gcc -c -o fputu.o fputu.c

putuchar.o: putuchar.c
	gcc -c -o putuchar.o putuchar.c

decode_utf8.o: decode_utf8.c
	gcc -c -o decode_utf8.o decode_utf8.c

isspaceu.o: isspaceu.c
	gcc -c -o isspaceu.o isspaceu.c

utod.exe: utod.c getu.o is1butf8.o isb1of2b.o istbutf8.o isb1of3b.o isb1of4b.o isb1ofu8.o 
	gcc -o utod.exe utod.c getu.o is1butf8.o isb1of2b.o istbutf8.o isb1of3b.o isb1of4b.o isb1ofu8.o 

utox.exe: utox.c getu.o putu.o is1butf8.o isb1of2b.o istbutf8.o is2butf8.o isb1of3b.o is3butf8.o isb1of4b.o is4butf8.o
	gcc -o utox.exe utox.c getu.o putu.o is1butf8.o isb1of2b.o istbutf8.o is2butf8.o isb1of3b.o is3butf8.o isb1of4b.o is4butf8.o

xtoU.exe: xtoU.c putuchar.o decode_utf8.o is1butf8.o isb1of2b.o istbutf8.o is2butf8.o isb1of3b.o is3butf8.o isb1of4b.o is4butf8.o
	gcc -o xtoU.exe xtoU.c putuchar.o decode_utf8.o is1butf8.o isb1of2b.o istbutf8.o is2butf8.o isb1of3b.o is3butf8.o isb1of4b.o is4butf8.o

