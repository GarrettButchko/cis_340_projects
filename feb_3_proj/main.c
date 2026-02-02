/*
    main.c
    Description: Reads 3 PokemonCard records from standard input and displays them
                 in two different output orders.
    Programmer: Garrett Butchko, Karm Patel, Daemon Williams
    Last Modified: 2026-02-02
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "pokemon_card.h"

#define CARD_COUNT 3

/*
    main
    Arguments: none
    Transformation: Creates an array of 3 PokemonCard records, fills them from user input,
                    and prints them in two output orders.
*/
int main(void) {
    PokemonCard cards[CARD_COUNT];

    int i = 0;
    for (i = 0; i < CARD_COUNT; i = i + 1){
        input_card(&cards[i], i);
    }

    print_cards_order1(cards, CARD_COUNT);
    print_cards_order2(cards, CARD_COUNT);

    return 0;
}

/*
    trim_newline
    Arguments: s - a mutable C-string
    Transformation: If s ends with '\n', replace that newline with '\0'.
*/
static void trim_newline(char *s) {
    if (!s) {
        return;
    } else { /* Professor Compliant */ }

    size_t n = strlen(s);

    if (n > 0) {
        if (s[n - 1] == '\n') {
            s[n - 1] = '\0';
        } else { /* Professor Compliant */ }
    } else { /* Professor Compliant */ }
}

/*
    read_line
    Arguments: prompt - message to show user
               out - output character array (where the line is stored)
               out_sz - size of out buffer
               requireWordOnly - if true, input must be a single word with no digits
    Transformation: Prompts until a non-empty line is read into out (no trailing '\n').
*/
static void read_line(const char *prompt, char *out, size_t out_sz, bool requireWordOnly) {
    for (;;) {
        printf("%s", prompt);
        fflush(stdout);

        if (!fgets(out, (int)out_sz, stdin)) {
            fprintf(stderr, "\nInput ended unexpectedly.\n");
            exit(EXIT_FAILURE);
        } else { /* Professor Compliant */ }

        trim_newline(out);

        if (out[0] == '\0') {
            printf("  Please enter a non-empty value.\n");
            continue;
        } else { /* Professor Compliant */ }

        if (requireWordOnly){
            int valid = 1;

            int i = 0;
            for (i = 0; out[i] != '\0'; i = i + 1) {
                if (isspace((unsigned char)out[i])) {
                    printf("Please enter a single word (no spaces).\n");
                    valid = 0;
                    break;
                } else { /* Professor Compliant */ }

                if (isdigit((unsigned char)out[i])) {
                    printf("Please enter a word that does not have digits.\n");
                    valid = 0;
                    break;
                } else { /* Professor Compliant */ }
            }

            if (!valid) {
                continue;   /* restart outer loop and prompt again */
            } else { /* Professor Compliant */ }
        } else { /* Professor Compliant */ }

        return;
    }
}

/*
    is_valid_int_string
    Arguments: s - input string
    Transformation: Returns 1 if s is a valid integer string (optional '-', digits only).
*/
static int is_valid_int_string(const char *s)
{
    if (!s || *s == '\0') {
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
    Arguments: s - input string
    Transformation: Returns 1 if s is a valid non-scientific decimal string:
                    optional '-', digits, and at most one '.'.
*/
static int is_valid_double_string(const char *s) {
    if (!s || *s == '\0') {
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
    getBuf
    Arguments: buf - output buffer to store input line
               buf_sz - size of buf
               prompt - prompt to display
    Transformation: Reads a line into buf using fgets and removes trailing newline.
*/
static void getBuf(char *buf, size_t buf_sz, const char *prompt) {
    printf("%s", prompt);
    fflush(stdout);

    if (!fgets(buf, (int)buf_sz, stdin)) {
        fprintf(stderr, "\nInput ended unexpectedly.\n");
        exit(EXIT_FAILURE);
    } else { /* Professor Compliant */ }

    trim_newline(buf);
}

/*
    read_int_atoi
    Arguments: prompt - prompt to display
    Transformation: Reads until a valid non-negative integer is entered; returns that integer.
*/
static int read_int_atoi(const char *prompt) {
    char buf[128];

    for (;;) {
        getBuf(buf, sizeof(buf), prompt);

        if (!is_valid_int_string(buf)) {
            printf("  Please enter a valid integer.\n");
            continue;
        } else { /* Professor Compliant */ }

        int v = atoi(buf);

        if (v < 0) {
            printf("  Please enter a non-negative integer.\n");
            continue;
        } else { /* Professor Compliant */ }

        return v;
    }
}

/*
    read_double_atof
    Arguments: prompt - prompt to display
    Transformation: Reads until a valid non-negative decimal is entered; returns that value.
*/
static double read_double_atof(const char *prompt)
{
    char buf[128];

    for (;;)
    {
        getBuf(buf, sizeof(buf), prompt);

        if (!is_valid_double_string(buf)) {
            printf("  Please enter a valid number (example: 12.50).\n");
            continue;
        } else { /* Professor Compliant */ }

        double v = atof(buf);

        if (v < 0.0) {
            printf("  Please enter a non-negative value.\n");
            continue;
        } else { /* Professor Compliant */ }

        return v;
    }
}

/*
    input_card
    Arguments: c - pointer to PokemonCard to fill
               idx - which card number this is (0-based)
    Transformation: Prompts user and fills all fields in *c.
*/
static void input_card(PokemonCard *c, int idx) {
    printf("\n=== Enter data for Pokemon card %d of %d ===\n", idx + 1, CARD_COUNT);

    read_line("Name: ", c->name, sizeof(c->name), false);
    read_line("Type: ", c->type, sizeof(c->type), true);

    c->hit_points = read_int_atoi("Hit points (integer): ");
    c->dollar_value = read_double_atof("Dollar value (e.g., 3.99): ");
}

/*
    print_cards_order1
    Arguments: cards - array of PokemonCard
               n - number of cards
    Transformation: Prints records in order: name, type, hit points, dollar value.
*/
static void print_cards_order1(const PokemonCard cards[], int n) {
    printf("\n--- Display Order 1: name, type, hit points, dollar value ---\n");

    int i = 0;
    for (i = 0; i < n; i = i + 1) {
        printf("Card %d: name=%s | type=%s | hit_points=%d | dollar_value=$%.2f\n",
               i + 1, cards[i].name, cards[i].type, cards[i].hit_points, cards[i].dollar_value);
    }
}

/*
    print_cards_order2
    Arguments: cards - array of PokemonCard
               n - number of cards
    Transformation: Prints records in order: dollar value, hit points, name, type.
*/
static void print_cards_order2(const PokemonCard cards[], int n) {
    printf("\n--- Display Order 2: dollar value, hit points, name, type ---\n");

    int i = 0;
    for (i = 0; i < n; i = i + 1) {
        printf("Card %d: dollar_value=$%.2f | hit_points=%d | name=%s | type=%s\n",
               i + 1, cards[i].dollar_value, cards[i].hit_points, cards[i].name, cards[i].type);
    }
}


