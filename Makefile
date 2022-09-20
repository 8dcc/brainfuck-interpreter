
CC=gcc
CFLAGS=-Wall

all: bf-interpreter.out

bf-interpreter.out: src/*.c src/*.h
	$(CC) $(CFLAGS) -o $@ src/main.c -lncurses

clean:
	rm *.out
