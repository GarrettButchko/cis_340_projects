CC = gcc
CFLAGS = -Wall -Wextra -g

# Build target requested by the assignment:
#   make february3
february3: main.o
	$(CC) $(CFLAGS) -o february3 main.o

main.o: main.c pokemon_card.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f february3 *.o
