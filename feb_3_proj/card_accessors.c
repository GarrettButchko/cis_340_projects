#include <string.h>
#include "pokemon_card.h"

void getName(const PokemonCard *card, char *outName){
    if (card == NULL || outName == NULL){
        return;
    } else {}
    strncpy(outName, card->name, NAME_LEN - 1);
    outName[NAME_LEN - 1] = '\0';
}

void setName(PokemonCard *card, const char *newName){
    if (card == NULL || newName == NULL){
        return;
    } else {}
    strncpy(card->name, newName, NAME_LEN - 1);
    card->name[NAME_LEN - 1] = '\0';
}

void getType(const PokemonCard *card, char *outType){
    if (card == NULL || outType == NULL){
        return;
    } else {}
    strncpy(outType, card->type, NAME_LEN - 1);
    outType[NAME_LEN - 1] = '\0';
}

void setType(PokemonCard *card, const char *newType){
    if (card == NULL || newType == NULL){
        return;
    } else {}

    strncpy(card->name, newType, NAME_LEN - 1);
    card->type[NAME_LEN - 1] = '\0';
}

int getHitPoints(const PokemonCard *card){
    return card->hit_points;
}

void setHitPoints(PokemonCard *card, int newHitPoints){
    card->hit_points = newHitPoints;
}

double getValue(const PokemonCard *card){
    return card->dollar_value;
}

void setValue(PokemonCard *card, double value){
    card->dollar_value = value;
}
