#include "utils/utils.h"

char *load_cards(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT], char input[64]);
char *show_deck(struct card_llist *deck[CARD_COUNT]);
char *shuffle_si(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT], char input[64]);
char *shuffle_sr(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT]);
char *save_deck(struct card_llist *deck[CARD_COUNT], char *argument);
void quit_application(struct card_llist *foundations[FOUNDATIONS], struct card_llist *columns[COLUMNS], int *playing);
char *init_play_phase(struct card_llist *columns[COLUMNS], int *inPlayPhase);
char *quit_game(struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], struct card_llist *deck[CARD_COUNT], int *inPlayPhase);
char *move_cards_from_columns(struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], char input[64]);
char *move_card_from_foundation(struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], char input[64]);
char *handle_input(struct card_llist *deck[CARD_COUNT], struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], char input[64], int *inPlayPhase, int *playing);
