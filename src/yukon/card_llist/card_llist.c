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

int get_cards_size(struct card_llist *cards)
{
    int size = 0;
    while (cards != NULL)
    {
        size++;
        cards = cards->next;
    }
    return size;
}

int move_cards(struct card_llist *from, struct card_llist **to, int index)
{
    if (from == NULL)
    {
        printf("No cards to move\n");
        return -1;
    }
    struct card_llist *card = get_card_by_index(from, index);
    if (card == NULL)
    {
        printf("No card found at index %d\n", index);
        return -1;
    }
    add_card(to, card->next);
    card->next = NULL;
    return 0;
}

int shuffle_cards(struct card_llist **cards)
{
    if (*cards == NULL)
    {
        printf("No cards to shuffle\n");
        return -1;
    }
    int size = get_cards_size(*cards);
    struct card_llist *newDeck = NULL;
    for (int i = 0; i < size; i++)
    {
        int randomIndex = rand() % (size - 1);
        struct card_llist *randomCard = get_card_by_index(*cards, randomIndex);
        struct card_llist *pileOne = randomCard->next;
        struct card_llist *pileTwo = *cards;
        *cards = pileOne;
        randomCard->next = NULL;

        newDeck = pileOne;
        while (1)
        {
            pileOne = pileOne->next;
            newDeck->next = pileTwo;
            if (pileOne == NULL)
                break;
            newDeck = newDeck->next;

            pileTwo = pileTwo->next;
            newDeck->next = pileOne;
            if (pileTwo == NULL)
                break;
            newDeck = newDeck->next;
        }
    }
    return 0;
}

int add_card(struct card_llist **cards, struct card_llist *card)
{
    if (*cards == NULL)
    {
        *cards = card;
        return 0;
    }
    struct card_llist *lastCard = get_last_card(*cards);
    lastCard->next = card;
    return 0;
}

int remove_cards(struct card_llist *cards)
{
    struct card_llist *nextCard;
    while (cards != NULL)
    {
        nextCard = cards->next;
        free(cards);
        cards = nextCard;
    }
    return 0;
}