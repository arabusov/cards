CC=gcc
CFLAGS=-ansi -pedantic -Wall -g -ggdb
LD=gcc
LDFLAGS=-g -ggdb

SRC=$(wildcard *.c)
HRD=$(wildcard *.h)
OBJ=$(patsubst %.c, %.o, $(SRC))

EXE=run_tests

all: $(EXE) $(OBJ) $(HDR) Makefile

$(EXE): $(OBJ) $(HDR) Makefile
	$(LD) $(LDFLAGS) -o $(EXE) $(OBJ)
%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
