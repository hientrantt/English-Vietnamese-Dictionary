CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: main

main: soundex.o dictionary.o main.o
	${CC} soundex.o dictionary.o main.o -o main -Linc -Ilib -lbt

soundex.o: soundex.c
	${CC} ${CFLAGS} soundex.c -Linc -Ilib -lbt

dictionary.o: dictionary.c
	${CC} ${CFLAGS} dictionary.c -Linc -Ilib -lbt

main.o: main.c
	${CC} ${CFLAGS} main.c -Linc -Ilib -lbt
# -L../lib -lbt -I../inc
clean:
	rm -f *.o *-
