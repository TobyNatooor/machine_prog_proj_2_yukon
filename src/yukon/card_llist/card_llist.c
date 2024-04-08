#include <stdio.h>
#include <stdlib.h>
#include "card_llist.h"

struct card_llist *get_last_card(struct card_llist *cards)
{
    if (cards == NULL)
        return NULL;
    while (cards->next != NULL)
    {
        cards = cards->next;
    }
    return cards;
}

struct card_llist *get_card_by_index(struct card_llist *cards, int index)
{
    for (int i = 0; i < index; i++)
    {
        if (cards == NULL)
            return NULL;
        cards = cards->next;
    }
    return cards;
}

// int move_cards(struct card_llist *from, struct card_llist *to, int index)
// {
//     struct card_llist *card = get_card_by_index(from, index);
//     if (card == NULL)
//     {
//         printf("No card found at index %d\n", index);
//         return -1;
//     }
//     add_card(to, card);
//     card->next = NULL;
//     return 0;
// }

int add_card(struct card_llist *column, struct card_llist *card)
{
    if (column == NULL)
        return -1;
    struct card_llist *lastCard = get_last_card(column);
    lastCard->next = card;
    return 0;
}

int remove_cards(struct card_llist *cards) {
    struct card_llist *nextCard;
    while (cards != NULL) {
        nextCard = cards->next;
        free(cards);
        cards = nextCard;
    }
    return 0;
}