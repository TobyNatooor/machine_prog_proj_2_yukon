
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
int load_cards_from_file(struct card_llist *columns[], struct card_llist *foundations[]);
// int load_cards_from_array(card_llist *columns[], card_llist *foundations[]);
struct card_llist *get_last_card(struct card_llist *column);
void add_card(struct card_llist *column, struct card_llist *card);
int face_value_to_int(char card_value);
char int_to_face_value(int card_value);
int start_game();