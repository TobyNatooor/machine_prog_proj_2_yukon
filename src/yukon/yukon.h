
#define FILE_NAME "cards.txt"
#define CARDS_NUM 52
#define FOUNDATIONS 4
#define COLUMNS 7
#define CARD_SIZE 2

enum suits
{
    CLUBS = 'C',
    DIAMONDS = 'D',
    HEARTS = 'H',
    SPADES = 'S'
};

struct card_llist
{
    int value;
    int hidden;
    enum suits suit;
    struct card_llist *next;
};

int display_game(struct card_llist *columns[], struct card_llist *foundations[]);
int arrange_cards(struct card_llist *columns[], struct card_llist *foundations[]);
int load_cards_from_file(struct card_llist *columns[]);
// int load_cards_from_array(card_llist *columns[], card_llist *foundations[]);
struct card_llist *get_last_card(struct card_llist *column);
int move_cards(struct card_llist *from, struct card_llist *to, int count);
int print_columns(struct card_llist *column[]);
void add_card(struct card_llist *column, struct card_llist *card);
int face_value_to_int(char card_value);
char int_to_face_value(int card_value);
int start_game();