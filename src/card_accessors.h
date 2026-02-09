#ifndef CARD_ACCESSORS_H
#define CARD_ACCESSORS_H

#include "pokemon_card.h"

void getName(const PokemonCard *card, char *outName);
void setName(PokemonCard *card, const char *newName);

void getType(const PokemonCard *card, char *outType);
void setType(PokemonCard *card, const char *newType);

int getHitPoints(const PokemonCard *card);
void setHitPoints(PokemonCard *card, int newHitPoints);

double getValue(const PokemonCard *card);
void setValue(PokemonCard *card, double value);

void toString(RecordStructure* theRecord, char* placeToPutTheString);
int getToStringLength(RecordStructure* theRecord);

#endif /* CARD_ACCESSORS_H */
