
CC=gcc
CFLAGS=-Wall
BIN=bf-interpreter

all: $(BIN).out

$(BIN).out: src/*.c src/*.h
	$(CC) $(CFLAGS) -o $@ src/main.c -lncurses

run: $(BIN).out
	./$<

clean:
	rm $(BIN).out
