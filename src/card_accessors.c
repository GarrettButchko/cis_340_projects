#include <stdio.h>
#include <string.h>
#include "card_accessors.h"

#define CARD_TO_STRING_FORMAT "Name: %s | Type: %s | Hit Points: %d | Dollar Value: $%.2f"

/*
    getName
    Arguments: card - source card, outName - destination name buffer
    Transformation: Copies card name into outName with null termination safety.
*/
void getName(const PokemonCard *card, char *outName) {
    if (card == NULL || outName == NULL) {
        return;
    } else { /* Professor Compliant */ }

    strncpy(outName, card->name, NAME_LEN - 1);
    outName[NAME_LEN - 1] = '\0';
}

/*
    setName
    Arguments: card - destination card, newName - new card name
    Transformation: Copies newName into card->name with null termination safety.
*/
void setName(PokemonCard *card, const char *newName) {
    if (card == NULL || newName == NULL) {
        return;
    } else { /* Professor Compliant */ }

    strncpy(card->name, newName, NAME_LEN - 1);
    card->name[NAME_LEN - 1] = '\0';
}

/*
    getType
    Arguments: card - source card, outType - destination type buffer
    Transformation: Copies card type into outType with null termination safety.
*/
void getType(const PokemonCard *card, char *outType) {
    if (card == NULL || outType == NULL) {
        return;
    } else { /* Professor Compliant */ }

    strncpy(outType, card->type, TYPE_LEN - 1);
    outType[TYPE_LEN - 1] = '\0';
}

/*
    setType
    Arguments: card - destination card, newType - new card type
    Transformation: Copies newType into card->type with null termination safety.
*/
void setType(PokemonCard *card, const char *newType) {
    if (card == NULL || newType == NULL) {
        return;
    } else { /* Professor Compliant */ }

    strncpy(card->type, newType, TYPE_LEN - 1);
    card->type[TYPE_LEN - 1] = '\0';
}

/*
    getHitPoints
    Arguments: card - source card
    Transformation: Returns hit points; returns 0 when card is NULL.
*/
int getHitPoints(const PokemonCard *card) {
    if (card == NULL) {
        return 0;
    } else { /* Professor Compliant */ }

    return card->hit_points;
}

/*
    setHitPoints
    Arguments: card - destination card, newHitPoints - new hit point value
    Transformation: Stores newHitPoints in card when card is not NULL.
*/
void setHitPoints(PokemonCard *card, int newHitPoints) {
    if (card == NULL) {
        return;
    } else { /* Professor Compliant */ }

    card->hit_points = newHitPoints;
}

/*
    getValue
    Arguments: card - source card
    Transformation: Returns dollar value; returns 0.0 when card is NULL.
*/
double getValue(const PokemonCard *card) {
    if (card == NULL) {
        return 0.0;
    } else { /* Professor Compliant */ }

    return card->dollar_value;
}

/*
    setValue
    Arguments: card - destination card, value - new dollar value
    Transformation: Stores value in card when card is not NULL.
*/
void setValue(PokemonCard *card, double value) {
    if (card == NULL) {
        return;
    } else { /* Professor Compliant */ }

    card->dollar_value = value;
}

/*
    getToStringLength
    Arguments: theRecord - card to measure
    Transformation: Returns required output length including null terminator.
*/
int getToStringLength(RecordStructure* theRecord) {
    if (theRecord == NULL) {
        return 1;
    } else { /* Professor Compliant */ }

    int base_length = snprintf(
        NULL,
        0,
        CARD_TO_STRING_FORMAT,
        theRecord->name,
        theRecord->type,
        theRecord->hit_points,
        theRecord->dollar_value
    );

    if (base_length < 0) {
        return 1;
    } else { /* Professor Compliant */ }

    return base_length + 1;
}

/*
    toString
    Arguments: theRecord - card to display, placeToPutTheString - destination char buffer
    Transformation: Writes a human-friendly card description into destination buffer.
*/
void toString(RecordStructure* theRecord, char* placeToPutTheString) {
    if (placeToPutTheString == NULL) {
        return;
    } else { /* Professor Compliant */ }

    if (theRecord == NULL) {
        placeToPutTheString[0] = '\0';
        return;
    } else { /* Professor Compliant */ }

    int needed_length = getToStringLength(theRecord);
    if (needed_length <= 0) {
        placeToPutTheString[0] = '\0';
    } else {
        snprintf(
            placeToPutTheString,
            (size_t)needed_length,
            CARD_TO_STRING_FORMAT,
            theRecord->name,
            theRecord->type,
            theRecord->hit_points,
            theRecord->dollar_value
        );
    }
}
