#include <stdio.h>
#include "card_llist.h"

struct card_llist *get_last_card(struct card_llist *column)
{
    if (column == NULL)
        return NULL;
    struct card_llist *lastCard = column;
    while (lastCard->next != NULL)
    {
        lastCard = lastCard->next;
    }
    return lastCard;
}

void add_card(struct card_llist *column, struct card_llist *card)
{
    if (column == NULL)
    {
        column = card;
        return;
    }
    while (column->next != NULL)
    {
        column = column->next;
    }
    column->next = card;
}

struct card_llist *get_card_by_index(struct card_llist *column, int index)
{
    struct card_llist *card = column;
    for (int i = 0; i < index; i++)
    {
        if (card == NULL)
            return NULL;
        card = card->next;
    }
    return card;
}

void print_cards(struct card_llist *column)
{
    struct card_llist *card = column;
    while (card != NULL)
    {
        printf("%c%c ", int_to_face_value(card->value), card->suit);
        card = card->next;
    }
}