
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

struct card_llist *get_last_card(struct card_llist *column);
void add_card(struct card_llist *column, struct card_llist *card);
struct card_llist *get_card_by_index(struct card_llist *column, int index);
void print_cards(struct card_llist *column);
