#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../model/card_llist/card_llist.h"

#define CARD_COUNT 52
#define CARD_SIZE 2
#define FOUNDATIONS 4
#define COLUMNS 7
#define DELIMITER '\n'

int load_cards_from_file(struct card_llist *deck[CARD_COUNT], char fileName[]);
int load_cards_from_array(struct card_llist *deck[CARD_COUNT], const char cards[CARD_COUNT][CARD_SIZE]);
int arrange_cards(struct card_llist *columns[COLUMNS]);
int deck_to_columns(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT]);
struct card_llist *deck_to_card_llist(struct card_llist *deck[CARD_COUNT]);
int show_after_index(struct card_llist *column, int index);
int won_game(struct card_llist *columns[COLUMNS]);
char *get_command(char input[64]);
char *get_argument(char input[64]);