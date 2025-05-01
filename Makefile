CC := /usr/bin/gcc

all: clean | task1 task2

task1:
	$(CC) -o 6-1.o src/6-1.c
task2:
	$(CC) -o 6-2.o src/6-2.c

clean: 
	rm -rf *.o
