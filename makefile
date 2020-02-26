OBJS = main.o cda.o queue.o maze.o da.o stack.o
OPTS = gcc -Wall -Wextra -g -std=c99 -ggdb3

all: amaze

amaze: main.o cda.o queue.o maze.o stack.o da.o
	$(OPTS) -o amaze maze.o main.o cda.o stack.o da.o queue.o

da.o: da.h da.c
	$(OPTS) -c da.c

cda.o: cda.h cda.c
	$(OPTS) -c cda.c

stack.o: stack.h da.h stack.c
	$(OPTS) -c stack.c

queue.o: queue.h cda.h queue.c
	$(OPTS) -c queue.c

maze.o: maze.h stack.h da.h queue.h cda.h maze.c
	$(OPTS) -c maze.c

main.o: main.c maze.h
	$(OPTS) -c main.c

clean: 
	rm -f *.o amaze

valgrind:
	valgrind --track-origins=yes --leak-check=full amaze -r 5794 -d ready.maze -c 3 3 unready.maze -s unready.maze ready.maze 
	
test:
	test amaze