CC = gcc
CFLAGS = -Wall -Wextra -g

# Build target requested by the assignment:
#   make february5
february5: main.o card_accessors.o
	$(CC) $(CFLAGS) -o february5 main.o card_accessors.o

main.o: main.c pokemon_card.h
	$(CC) $(CFLAGS) -c main.c

card_accessors.o: card_accessors.c card_accessors.h pokemon_card.h
	$(CC) $(CFLAGS) -c card_accessors.c

clean:
	rm -f february5 *.o
