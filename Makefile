CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
DEPS = structures.h list_utils.h
OBJ = list_utils.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(OBJ)

test: test.o list_utils.o
	$(CC) -o test_runner $^ $(CFLAGS)
	./test_runner

clean:
	rm -f *.o test_runner