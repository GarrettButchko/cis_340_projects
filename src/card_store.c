#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "card_store.h"

#define INPUT_BUFFER_LEN 512
#define INITIAL_CAPACITY 4
#define FIELD_DELIMITER '|'

/*
    trim_newline
    Arguments: s - mutable C string
    Transformation: Removes trailing newline from s when present.
*/
static void trim_newline(char *s) {
    if (s == NULL) {
        return;
    } else { /* Professor Compliant */ }

    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') {
        s[n - 1] = '\0';
    } else { /* Professor Compliant */ }
}

/*
    flush_remainder_of_line
    Arguments: stream - input stream with potentially truncated current line
    Transformation: Discards remaining bytes until newline or EOF.
*/
static void flush_remainder_of_line(FILE *stream) {
    if (stream == NULL) {
        return;
    } else { /* Professor Compliant */ }

    int ch = 0;
    for (;;) {
        ch = fgetc(stream);
        if (ch == '\n' || ch == EOF) {
            break;
        } else { /* Professor Compliant */ }
    }
}

/*
    is_valid_int_string
    Arguments: s - candidate integer string
    Transformation: Returns 1 for optional-leading-minus integer text; else returns 0.
*/
static int is_valid_int_string(const char *s) {
    if (s == NULL || s[0] == '\0') {
        return 0;
    } else { /* Professor Compliant */ }

    int i = 0;
    if (s[i] == '-') {
        i = i + 1;
    } else { /* Professor Compliant */ }

    if (s[i] == '\0') {
        return 0;
    } else { /* Professor Compliant */ }

    for (; s[i] != '\0'; i = i + 1) {
        if (!isdigit((unsigned char)s[i])) {
            return 0;
        } else { /* Professor Compliant */ }
    }

    return 1;
}

/*
    is_valid_double_string
    Arguments: s - candidate decimal string
    Transformation: Returns 1 for optional-leading-minus number with at most one dot.
*/
static int is_valid_double_string(const char *s) {
    if (s == NULL || s[0] == '\0') {
        return 0;
    } else { /* Professor Compliant */ }

    int i = 0;
    int seen_digit = 0;
    int seen_dot = 0;

    if (s[i] == '-') {
        i = i + 1;
    } else { /* Professor Compliant */ }

    for (; s[i] != '\0'; i = i + 1) {
        if (isdigit((unsigned char)s[i])) {
            seen_digit = 1;
        } else if (s[i] == '.' && !seen_dot) {
            seen_dot = 1;
        } else {
            return 0;
        }
    }

    return seen_digit;
}

/*
    copy_card_fields
    Arguments: dest - destination card
               name - source name text
               type - source type text
               hit_points - source hit point value
               value - source dollar value
    Transformation: Copies provided fields into a PokemonCard safely.
*/
static void copy_card_fields(PokemonCard *dest, const char *name, const char *type, int hit_points, double value) {
    if (dest == NULL || name == NULL || type == NULL) {
        return;
    } else { /* Professor Compliant */ }

    strncpy(dest->name, name, NAME_LEN - 1);
    dest->name[NAME_LEN - 1] = '\0';

    strncpy(dest->type, type, TYPE_LEN - 1);
    dest->type[TYPE_LEN - 1] = '\0';

    dest->hit_points = hit_points;
    dest->dollar_value = value;
}

/*
    parse_card_line
    Arguments: line - one file line using name|type|hit_points|dollar_value
               out_card - parsed card output
    Transformation: Parses and validates a single line; returns 1 for success, 0 otherwise.
*/
static int parse_card_line(const char *line, PokemonCard *out_card) {
    if (line == NULL || out_card == NULL) {
        return 0;
    } else { /* Professor Compliant */ }

    char work[INPUT_BUFFER_LEN];
    strncpy(work, line, sizeof(work) - 1);
    work[sizeof(work) - 1] = '\0';

    char *d1 = strchr(work, FIELD_DELIMITER);
    if (d1 == NULL) {
        return 0;
    } else { /* Professor Compliant */ }
    *d1 = '\0';

    char *d2 = strchr(d1 + 1, FIELD_DELIMITER);
    if (d2 == NULL) {
        return 0;
    } else { /* Professor Compliant */ }
    *d2 = '\0';

    char *d3 = strchr(d2 + 1, FIELD_DELIMITER);
    if (d3 == NULL) {
        return 0;
    } else { /* Professor Compliant */ }
    *d3 = '\0';

    if (strchr(d3 + 1, FIELD_DELIMITER) != NULL) {
        return 0;
    } else { /* Professor Compliant */ }

    const char *name = work;
    const char *type = d1 + 1;
    const char *hp_str = d2 + 1;
    const char *value_str = d3 + 1;

    if (name[0] == '\0' || type[0] == '\0' || hp_str[0] == '\0' || value_str[0] == '\0') {
        return 0;
    } else { /* Professor Compliant */ }

    if (strlen(name) >= NAME_LEN || strlen(type) >= TYPE_LEN) {
        return 0;
    } else { /* Professor Compliant */ }

    if (!is_valid_int_string(hp_str) || !is_valid_double_string(value_str)) {
        return 0;
    } else { /* Professor Compliant */ }

    int hp = atoi(hp_str);
    double value = atof(value_str);

    if (hp < 0 || value < 0.0) {
        return 0;
    } else { /* Professor Compliant */ }

    copy_card_fields(out_card, name, type, hp, value);
    return 1;
}

/*
    ensure_capacity
    Arguments: cards - pointer to dynamic array pointer
               capacity - current capacity
               needed_count - required element count
    Transformation: Grows array via realloc until needed_count fits; returns 1 on success.
*/
int ensure_capacity(PokemonCard **cards, int *capacity, int needed_count) {
    if (cards == NULL || capacity == NULL) {
        return 0;
    } else { /* Professor Compliant */ }

    if (needed_count <= *capacity) {
        return 1;
    } else { /* Professor Compliant */ }

    int new_capacity = *capacity;
    if (new_capacity <= 0) {
        new_capacity = INITIAL_CAPACITY;
    } else { /* Professor Compliant */ }

    while (new_capacity < needed_count) {
        new_capacity = new_capacity * 2;
    }

    PokemonCard *resized = realloc(*cards, (size_t)new_capacity * sizeof(PokemonCard));
    if (resized == NULL) {
        return 0;
    } else { /* Professor Compliant */ }

    *cards = resized;
    *capacity = new_capacity;
    return 1;
}

/*
    find_card_index_by_name
    Arguments: cards - card array
               count - number of cards
               name_key - name to match
    Transformation: Returns index of first exact name match, else -1.
*/
int find_card_index_by_name(const PokemonCard cards[], int count, const char *name_key) {
    if (cards == NULL || name_key == NULL) {
        return -1;
    } else { /* Professor Compliant */ }

    int i = 0;
    for (i = 0; i < count; i = i + 1) {
        if (strcmp(cards[i].name, name_key) == 0) {
            return i;
        } else { /* Professor Compliant */ }
    }

    return -1;
}

/*
    writeCardsToFile
    Arguments: path - destination file path
               cards - card array
               count - number of cards to write
    Transformation: Writes cards in line format name|type|hit_points|dollar_value.
                    Returns 1 for success and 0 for failure.
*/
int writeCardsToFile(const char *path, const PokemonCard cards[], int count) {
    if (path == NULL) {
        return 0;
    } else { /* Professor Compliant */ }

    if (count > 0 && cards == NULL) {
        return 0;
    } else { /* Professor Compliant */ }

    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        return 0;
    } else { /* Professor Compliant */ }

    int i = 0;
    for (i = 0; i < count; i = i + 1) {
        if (fprintf(
                fp,
                "%s|%s|%d|%.2f\n",
                cards[i].name,
                cards[i].type,
                cards[i].hit_points,
                cards[i].dollar_value
            ) < 0) {
            fclose(fp);
            return 0;
        } else { /* Professor Compliant */ }
    }

    if (fclose(fp) != 0) {
        return 0;
    } else { /* Professor Compliant */ }

    return 1;
}

/*
    readCardsFromFile
    Arguments: path - source file path
               cards - pointer to dynamic array pointer
               count - pointer to current count
               capacity - pointer to current capacity
               file_found - output flag; 1 if file opened, 0 if missing/unopenable
    Transformation: Reads cards using fgets and parses line format
                    name|type|hit_points|dollar_value.
                    Malformed lines are skipped.
                    Returns number loaded, or -1 on memory/setup failure.
*/
int readCardsFromFile(
    const char *path,
    PokemonCard **cards,
    int *count,
    int *capacity,
    int *file_found
) {
    if (path == NULL || cards == NULL || count == NULL || capacity == NULL || file_found == NULL) {
        return -1;
    } else { /* Professor Compliant */ }

    *file_found = 0;

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        return 0;
    } else { /* Professor Compliant */ }

    *file_found = 1;

    char line[INPUT_BUFFER_LEN];
    int loaded_count = 0;
    int skipped_count = 0;

    for (;;) {
        if (fgets(line, sizeof(line), fp) == NULL) {
            break;
        } else { /* Professor Compliant */ }

        if (strchr(line, '\n') == NULL) {
            flush_remainder_of_line(fp);
            skipped_count = skipped_count + 1;
            continue;
        } else { /* Professor Compliant */ }

        trim_newline(line);

        if (line[0] == '\0') {
            continue;
        } else { /* Professor Compliant */ }

        PokemonCard parsed_card;
        if (!parse_card_line(line, &parsed_card)) {
            skipped_count = skipped_count + 1;
            continue;
        } else { /* Professor Compliant */ }

        if (!ensure_capacity(cards, capacity, *count + 1)) {
            fclose(fp);
            return -1;
        } else { /* Professor Compliant */ }

        (*cards)[*count] = parsed_card;
        *count = *count + 1;
        loaded_count = loaded_count + 1;
    }

    fclose(fp);

    if (skipped_count > 0) {
        printf("Skipped %d malformed line(s) while reading %s.\n", skipped_count, path);
    } else { /* Professor Compliant */ }

    return loaded_count;
}
