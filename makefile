is1butf8.o: is1butf8.c
	gcc -c -o is1butf8.o is1butf8.c

is2butf8.o: is2butf8.c
	gcc -c -o is2butf8.o is2butf8.c 

is3butf8.o: is3butf8.c
	gcc -c -o is3butf8.o is3butf8.c

is4butf8.o: is4butf8.c
	gcc -c -o is4butf8.o is4butf8.c

putu.o: putu.c is1butf8.o is2butf8.o is3butf8.o is4butf8.o
	gcc -c -o putu.o putu.c is1butf8.o is2butf8.o is3butf8.o is4butf8.o

strlenu8.o: strlenu8.c
	gcc -c -o strlenu8.o strlenu8.c

toloweru.o: toloweru.c
	gcc -c -o toloweru.o toloweru.c is1butf8.o is2butf8.o is3butf8.o is4butf8.o
