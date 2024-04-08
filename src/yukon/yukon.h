#include "card_llist/card_llist.h"

#define FILE_NAME "cards.txt"
#define CARDS_NUM 52
#define FOUNDATIONS 4
#define COLUMNS 7
#define CARD_SIZE 2

int display_game(struct card_llist *columns[], struct card_llist *foundations[]);
int arrange_cards(struct card_llist *columns[], struct card_llist *foundations[]);
int load_cards_from_file(struct card_llist *columns[]);
int load_cards_from_array(struct card_llist *columns[], const char cards[CARDS_NUM][2]);
int print_columns(struct card_llist *column[]);
int face_value_to_int(char card_value);
char int_to_face_value(int card_value);
int start_game();