#ifndef POKEMON_CARD_H
#define POKEMON_CARD_H

#define NAME_LEN 64
#define TYPE_LEN 32

typedef struct {
    char name[NAME_LEN];
    char type[TYPE_LEN];
    int hit_points;
    double dollar_value;
} PokemonCard;

/*
    RecordStructure
    Transformation: Compatibility typedef required by assignment-provided signatures.
*/
typedef PokemonCard RecordStructure;

#endif /* POKEMON_CARD_H */
