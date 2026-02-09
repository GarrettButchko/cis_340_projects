/*
    main.c
    Description: Maintains a dynamic list of Pokemon cards that can be loaded from
                 and saved to a text file. Supports listing, adding, and deleting cards.
    Programmer: Garrett Butchko, Karm Patel, Daemon Williams (updated)
    Last Modified: 2026-02-09
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "pokemon_card.h"
#include "card_accessors.h"
#include "card_store.h"

#define DEFAULT_CARD_FILE "cards.txt"
#define FIELD_DELIMITER '|'

static void trim_newline(char *s);
static void flush_remainder_of_line(FILE *stream);
static int contains_character(const char *s, char ch);
static int is_valid_int_string(const char *s);
static int is_valid_double_string(const char *s);
static void read_line_with_rules(const char *prompt, char *out, size_t out_sz, bool reject_delimiter);
static int read_int_atoi(const char *prompt);
static double read_double_atof(const char *prompt);
static int prompt_menu_choice(void);
static void list_cards(const PokemonCard cards[], int count);
static void add_card_interactive(PokemonCard **cards, int *count, int *capacity);
static void delete_card_interactive(PokemonCard cards[], int *count);

/*
    main
    Arguments: none
    Transformation: Initializes a dynamic card list, loads cards from disk if available,
                    runs an interactive menu loop, and frees memory before exiting.
*/
int main(void) {
    PokemonCard *cards = NULL;
    int count = 0;
    int capacity = 0;
    int file_found = 0;

    int loaded_count = readCardsFromFile(
        DEFAULT_CARD_FILE,
        &cards,
        &count,
        &capacity,
        &file_found
    );

    if (loaded_count < 0) {
        fprintf(stderr, "Unable to initialize card list from file.\n");
        free(cards);
        return EXIT_FAILURE;
    } else { /* Professor Compliant */ }

    if (file_found) {
        printf("Loaded %d card(s) from %s.\n", loaded_count, DEFAULT_CARD_FILE);
    } else {
        printf("No existing file at %s. Starting with an empty list.\n", DEFAULT_CARD_FILE);
    }

    for (;;) {
        int choice = prompt_menu_choice();

        if (choice == 1) {
            list_cards(cards, count);
        } else if (choice == 2) {
            add_card_interactive(&cards, &count, &capacity);
        } else if (choice == 3) {
            delete_card_interactive(cards, &count);
        } else if (choice == 4) {
            if (writeCardsToFile(DEFAULT_CARD_FILE, cards, count)) {
                printf("Saved %d card(s) to %s.\n", count, DEFAULT_CARD_FILE);
            } else {
                printf("Save failed.\n");
            }
        } else if (choice == 5) {
            if (writeCardsToFile(DEFAULT_CARD_FILE, cards, count)) {
                printf("Saved %d card(s) to %s.\n", count, DEFAULT_CARD_FILE);
            } else {
                printf("Save failed. Exiting anyway.\n");
            }
            break;
        } else if (choice == 6) {
            printf("Exiting without saving.\n");
            break;
        } else {
            printf("Please enter a menu option from 1 to 6.\n");
        }
    }

    free(cards);
    cards = NULL;

    return 0;
}

/*
    trim_newline
    Arguments: s - mutable C string
    Transformation: Removes trailing '\n' from s if one exists.
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
    Arguments: stream - input stream currently positioned after a partial fgets read
    Transformation: Reads and discards bytes until newline or EOF.
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
    contains_character
    Arguments: s - input string, ch - character to locate
    Transformation: Returns 1 if ch appears in s, else 0.
*/
static int contains_character(const char *s, char ch) {
    if (s == NULL) {
        return 0;
    } else { /* Professor Compliant */ }

    int i = 0;
    for (i = 0; s[i] != '\0'; i = i + 1) {
        if (s[i] == ch) {
            return 1;
        } else { /* Professor Compliant */ }
    }

    return 0;
}

/*
    is_valid_int_string
    Arguments: s - input C string
    Transformation: Returns 1 if s is an integer format with optional leading '-', else 0.
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
    Arguments: s - input C string
    Transformation: Returns 1 if s is a decimal-like number with optional '-' and one '.',
                    else returns 0.
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
    read_line_with_rules
    Arguments: prompt - text shown to user
               out - destination character buffer
               out_sz - destination size
               reject_delimiter - when true, rejects FIELD_DELIMITER in input
    Transformation: Prompts until a non-empty valid line is read with fgets.
*/
static void read_line_with_rules(const char *prompt, char *out, size_t out_sz, bool reject_delimiter) {
    for (;;) {
        printf("%s", prompt);
        fflush(stdout);

        if (fgets(out, (int)out_sz, stdin) == NULL) {
            fprintf(stderr, "\nInput ended unexpectedly.\n");
            exit(EXIT_FAILURE);
        } else { /* Professor Compliant */ }

        if (strchr(out, '\n') == NULL) {
            flush_remainder_of_line(stdin);
        } else { /* Professor Compliant */ }

        trim_newline(out);

        if (out[0] == '\0') {
            printf("Please enter a non-empty value.\n");
            continue;
        } else { /* Professor Compliant */ }

        if (reject_delimiter && contains_character(out, FIELD_DELIMITER)) {
            printf("Character '%c' is not allowed in this field.\n", FIELD_DELIMITER);
            continue;
        } else { /* Professor Compliant */ }

        return;
    }
}

/*
    read_int_atoi
    Arguments: prompt - user prompt string
    Transformation: Prompts until a valid non-negative integer is entered.
*/
static int read_int_atoi(const char *prompt) {
    char buf[128];

    for (;;) {
        read_line_with_rules(prompt, buf, sizeof(buf), false);

        if (!is_valid_int_string(buf)) {
            printf("Please enter a valid integer.\n");
            continue;
        } else { /* Professor Compliant */ }

        int value = atoi(buf);
        if (value < 0) {
            printf("Please enter a non-negative integer.\n");
            continue;
        } else { /* Professor Compliant */ }

        return value;
    }
}

/*
    read_double_atof
    Arguments: prompt - user prompt string
    Transformation: Prompts until a valid non-negative decimal number is entered.
*/
static double read_double_atof(const char *prompt) {
    char buf[128];

    for (;;) {
        read_line_with_rules(prompt, buf, sizeof(buf), false);

        if (!is_valid_double_string(buf)) {
            printf("Please enter a valid number (example: 12.50).\n");
            continue;
        } else { /* Professor Compliant */ }

        double value = atof(buf);
        if (value < 0.0) {
            printf("Please enter a non-negative value.\n");
            continue;
        } else { /* Professor Compliant */ }

        return value;
    }
}

/*
    prompt_menu_choice
    Arguments: none
    Transformation: Displays menu options and returns validated integer input.
*/
static int prompt_menu_choice(void) {
    printf("\n=== Pokemon Card Manager ===\n");
    printf("1) List cards\n");
    printf("2) Add card\n");
    printf("3) Delete card (by name, first match)\n");
    printf("4) Save\n");
    printf("5) Save & Exit\n");
    printf("6) Exit without saving\n");

    return read_int_atoi("Choose an option: ");
}

/*
    list_cards
    Arguments: cards - array of cards
               count - number of cards in array
    Transformation: Prints each card using toString(), or prints empty-state message.
*/
static void list_cards(const PokemonCard cards[], int count) {
    if (count <= 0) {
        printf("No cards to display.\n");
        return;
    } else { /* Professor Compliant */ }

    int i = 0;
    for (i = 0; i < count; i = i + 1) {
        int needed_length = getToStringLength((RecordStructure *)&cards[i]);

        if (needed_length <= 0) {
            needed_length = 1;
        } else { /* Professor Compliant */ }

        char *text = malloc((size_t)needed_length);
        if (text == NULL) {
            printf("%d) [Unable to allocate display buffer]\n", i + 1);
        } else {
            toString((RecordStructure *)&cards[i], text);
            printf("%d) %s\n", i + 1, text);
            free(text);
        }
    }
}

/*
    add_card_interactive
    Arguments: cards - pointer to dynamic card array pointer
               count - card count
               capacity - card capacity
    Transformation: Prompts for one card and appends it to dynamic array.
*/
static void add_card_interactive(PokemonCard **cards, int *count, int *capacity) {
    if (cards == NULL || count == NULL || capacity == NULL) {
        printf("Internal error: invalid card storage.\n");
        return;
    } else { /* Professor Compliant */ }

    if (!ensure_capacity(cards, capacity, *count + 1)) {
        printf("Unable to allocate memory for another card.\n");
        return;
    } else { /* Professor Compliant */ }

    PokemonCard new_card;
    printf("\nEnter card details:\n");
    read_line_with_rules("Name: ", new_card.name, sizeof(new_card.name), true);
    read_line_with_rules("Type: ", new_card.type, sizeof(new_card.type), true);
    new_card.hit_points = read_int_atoi("Hit points (integer): ");
    new_card.dollar_value = read_double_atof("Dollar value (e.g., 3.99): ");

    (*cards)[*count] = new_card;
    *count = *count + 1;

    printf("Card added. Total cards: %d\n", *count);
}

/*
    delete_card_interactive
    Arguments: cards - card array
               count - pointer to card count
    Transformation: Prompts for a name key and deletes first matching card by shifting.
*/
static void delete_card_interactive(PokemonCard cards[], int *count) {
    if (count == NULL) {
        printf("Internal error: invalid card storage.\n");
        return;
    } else { /* Professor Compliant */ }

    if (*count <= 0) {
        printf("No cards to delete.\n");
        return;
    } else { /* Professor Compliant */ }

    if (cards == NULL) {
        printf("Internal error: invalid card storage.\n");
        return;
    } else { /* Professor Compliant */ }

    char name_key[NAME_LEN];
    read_line_with_rules("Enter card name to delete (first exact match): ", name_key, sizeof(name_key), true);

    int index = find_card_index_by_name(cards, *count, name_key);
    if (index < 0) {
        printf("No card found with name \"%s\".\n", name_key);
        return;
    } else { /* Professor Compliant */ }

    int i = index;
    for (i = index; i < *count - 1; i = i + 1) {
        cards[i] = cards[i + 1];
    }

    *count = *count - 1;
    printf("Deleted first match for \"%s\". Remaining cards: %d\n", name_key, *count);
}
