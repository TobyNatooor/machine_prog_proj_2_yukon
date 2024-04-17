#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "card_llist/card_llist.h"

#define CARD_COUNT 52
#define CARD_SIZE 2
#define FOUNDATIONS 4
#define COLUMNS 7
#define DELIMITER '\n'

struct card_llist *load_cards_from_file(char fileName[]);
struct card_llist *load_cards_from_array(const char cards[CARD_COUNT][CARD_SIZE]);
int arrange_cards(struct card_llist *columns[COLUMNS]);
int deck_to_columns(struct card_llist *columns[COLUMNS], struct card_llist *deck);
struct card_llist *columns_to_deck(struct card_llist *columns[COLUMNS]);
int show_after_index(struct card_llist *column, int index);
int get_input(char input[], char inputArg[]);
int won_game(struct card_llist *columns[COLUMNS]);
