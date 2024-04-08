#include "card_llist/card_llist.h"

#define FILE_NAME "cards.txt"
#define CARD_COUNT 52
#define CARD_SIZE 2
#define FOUNDATIONS 4
#define COLUMNS 7

int display_game(struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS]);
int arrange_cards(struct card_llist *columns[COLUMNS]);
int load_cards_from_file(struct card_llist *columns[COLUMNS]);
int load_cards_from_array(struct card_llist *columns[COLUMNS], const char cards[CARD_COUNT][CARD_SIZE]);
int print_columns(struct card_llist *column[COLUMNS]);
int face_value_to_int(char card_value);
char int_to_face_value(int card_value);
int start_game();