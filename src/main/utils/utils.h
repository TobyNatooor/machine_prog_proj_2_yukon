#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../model/card_llist.h"
#include "../defines.h"

int load_cards_from_file(struct card_llist *deck[CARD_COUNT], char fileName[]);
int load_cards_from_array(struct card_llist *deck[CARD_COUNT], const char cards[CARD_COUNT][CARD_SIZE]);
int arrange_cards(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT]);
int deck_to_columns(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT]);
struct card_llist *deck_to_card_llist(struct card_llist *deck[CARD_COUNT]);
int show_after_index(struct card_llist *column, int index);
int won_game(struct card_llist *columns[COLUMNS]);
char *get_command(char *input);
char *get_argument(char *input);
int checkDuplicate(struct card_llist *deck[CARD_COUNT], int index, char faceValue, char suit);