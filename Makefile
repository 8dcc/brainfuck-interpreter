
CC=gcc
CFLAGS=-Wall
BIN=bf-interpreter.out

.PHONY: clean all run

all: $(BIN)

$(BIN): src/*.c src/*.h
	$(CC) $(CFLAGS) -o $@ src/main.c -lncurses

run: $(BIN)
	./$<

clean:
	rm $(BIN)
