CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
DEPS = structures.h system_utils.h list_utils.h operations.h
OBJ = main.o system_utils.o list_utils.o operations.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(OBJ)
	$(CC) -o tema1 $^ $(CFLAGS)

run: build
	./tema1

clean:
	rm -f *.o tema1
	rm -f tema1.in tema1.out
	rm -f rezultat_valgrind.txt