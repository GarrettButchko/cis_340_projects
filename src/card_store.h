#ifndef CARD_STORE_H
#define CARD_STORE_H

#include "pokemon_card.h"

int ensure_capacity(PokemonCard **cards, int *capacity, int needed_count);
int find_card_index_by_name(const PokemonCard cards[], int count, const char *name_key);
int writeCardsToFile(const char *path, const PokemonCard cards[], int count);
int readCardsFromFile(
    const char *path,
    PokemonCard **cards,
    int *count,
    int *capacity,
    int *file_found
);

#endif /* CARD_STORE_H */
