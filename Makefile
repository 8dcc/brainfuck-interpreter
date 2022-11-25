
CC=gcc
CFLAGS=-Wall
LDFLAGS=-lncurses
OBJFILES=main.o interface.o commands.o brainfuck.o misc.o
BIN=bf-interpreter.out

all: $(BIN)

$(BIN): $(OBJFILES)
	$(CC) $(CFLAGS) -o $@ $(OBJFILES) $(LDFLAGS)

# https://makefiletutorial.com/#multiple-targets
# https://www.gnu.org/software/make/manual/html_node/Text-Functions.html
$(OBJFILES):
	$(CC) $(CFLAGS) -c src/$(@:.o=.c)

run: $(BIN)
	./$<

clean:
	rm -f $(BIN) $(OBJFILES)
