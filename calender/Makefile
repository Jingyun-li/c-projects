CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
		 -pedantic-errors -fstack-protector-all -Wextra
         
PROGS =  public01 public02 public03 public04 public05 \
	public06 student_tests

.PHONY: clean all

all: $(PROGS)

calendar.o: calendar.c calendar.h event.h
	$(CC) $(CFLAGS) -c calendar.c -o calendar.o

public01.o: public01.c 
	$(CC) $(CFLAGS) -c public01.c -o public01.o
	
public01: calendar.o public01.o my_memory_checker_216.o
	$(CC) calendar.o public01.o my_memory_checker_216.o -o public01
	
public02.o: public02.c 
	$(CC) $(CFLAGS) -c public02.c -o public02.o

public02: calendar.o public02.o my_memory_checker_216.o
	$(CC) calendar.o public02.o my_memory_checker_216.o -o public02

public03.o: public03.c 
	$(CC) $(CFLAGS) -c public03.c -o public03.o

public03: calendar.o public03.o my_memory_checker_216.o
	$(CC) calendar.o public03.o my_memory_checker_216.o -o public03
	
public04.o: public04.c 
	$(CC) $(CFLAGS) -c public04.c -o public04.o

public04: calendar.o public04.o my_memory_checker_216.o
	$(CC) calendar.o public04.o my_memory_checker_216.o -o public04
	
public05.o: public05.c 
	$(CC) $(CFLAGS) -c public05.c -o public05.o

public05: calendar.o public05.o my_memory_checker_216.o
	$(CC) calendar.o public05.o my_memory_checker_216.o -o public05
	
public06.o: public06.c 
	$(CC) $(CFLAGS) -c public06.c -o public06.o

public06: calendar.o public06.o my_memory_checker_216.o
	$(CC) calendar.o public06.o my_memory_checker_216.o -o public06

student_tests.o: student_tests.c 
	$(CC) $(CFLAGS) -c student_tests.c -o student_tests.o

student_tests: calendar.o student_tests.o my_memory_checker_216.o
	$(CC) calendar.o student_tests.o my_memory_checker_216.o -o student_tests

my_memory_checker_216.o: my_memory_checker_216.c
	$(CC) $(CFLAGS) -c my_memory_checker_216.c -o my_memory_checker_216.o

clean:
	@echo "Cleaning my system"
	@rm -f *.o calendar public01 public02 public03 public04 public05 \
		public06 student_tests

