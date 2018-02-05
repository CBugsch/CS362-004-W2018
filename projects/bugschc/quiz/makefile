CFLAGS = -Wall -fpic -lm -std=c99

default: prog run

prog: testme.c
	gcc -o testme testme.c $(CFLAGS)

run: 
	./testme
