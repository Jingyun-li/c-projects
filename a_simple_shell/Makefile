CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
	-pedantic-errors -fstack-protector-all 

PROGS = d8sh

.PHONY: all clean

d8sh: d8sh.o lexer.o executor.o parser.tab.o
	$(CC) $(CFLAGS) d8sh.o lexer.o executor.o parser.tab.o -o d8sh -lreadline

d8sh.o: d8sh.c executor.h lexer.h
	$(CC) $(CFLAGS) -c d8sh.c

lexer.o: lexer.c
	$(CC) $(CFLAGS) -c lexer.c

executor.o: executor.c executor.h command.h
	$(CC) $(CFLAGS) -c executor.c

parser.tab.o: parser.tab.c command.h
	$(CC) $(CFLAGS) -c parser.tab.c


all: $(PROGS)

clean:
	rm -f *.o d8sh
