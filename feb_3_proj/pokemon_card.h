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

static void trim_newline(char *s);
static void read_line(const char *prompt, char *out, size_t out_sz, bool requireWordOnly);
static int is_valid_int_string(const char *s);
static int is_valid_double_string(const char *s);
static void getBuf(char *buf, size_t buf_sz, const char *prompt);
static int read_int_atoi(const char *prompt);
static double read_double_atof(const char *prompt);
static void input_card(PokemonCard *c, int idx);
static void print_cards_order1(const PokemonCard cards[], int n);
static void print_cards_order2(const PokemonCard cards[], int n);

#endif /* POKEMON_CARD_H */
