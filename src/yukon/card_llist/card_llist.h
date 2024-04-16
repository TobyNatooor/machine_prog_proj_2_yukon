
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

struct card_llist *get_last_card(struct card_llist *cards);
struct card_llist *get_card_by_index(struct card_llist *cards, int index);
int get_card_index(struct card_llist *cards, int value, enum suits suit);
int get_cards_size(struct card_llist *cards);
int move_cards(struct card_llist *from, struct card_llist **to, int count);
int split_shuffle(struct card_llist **deck, int intersectionIndex);
int insert_shuffle(struct card_llist **deck);
int add_card(struct card_llist **cards, struct card_llist *card);
int remove_cards(struct card_llist *cards);