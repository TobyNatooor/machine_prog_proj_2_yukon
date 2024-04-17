#include "utils/utils.h"

char *load_cards(struct card_llist *columns[COLUMNS], char input[64]);
char *shuffle_si(struct card_llist *columns[COLUMNS], char input[64]);
char *save_deck(struct card_llist *columns[COLUMNS], char input[64]);
char *show_deck(struct card_llist *columns[COLUMNS]);
char *shuffle_sr(struct card_llist *columns[COLUMNS]);
char *move_cards_from_input(struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], char input[64]);
char *move_card_from_foundation(struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], char input[64]);
char *handle_input(struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], char input[64], int *inPlayPhase, int *playing);
