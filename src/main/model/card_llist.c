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

int get_card_index(struct card_llist *cards, int value, enum suits suit)
{
    int index = 0;
    while (cards != NULL)
    {
        if (cards->value == value && cards->suit == suit)
            return index;
        index++;
        cards = cards->next;
    }
    return -1;
}

int move_cards(struct card_llist **from, struct card_llist **to, int index)
{
    if (*from == NULL)
        return -1;
    
    if (index == 0)
    {
        add_card(to, *from);
        *from = NULL;
        return 0;
    }
    struct card_llist *card = get_card_by_index(*from, index - 1);
    if (card == NULL)
        return -1;
    
    add_card(to, card->next);
    card->next = NULL;
    return 0;
}

int split_shuffle(struct card_llist **deck, int intersectionIndex)
{
    if (*deck == NULL)
        return -1;
    
    int size = get_cards_size(*deck);
    struct card_llist *newDeck = NULL;
    struct card_llist *randomCard = get_card_by_index(*deck, intersectionIndex);
    struct card_llist *pileOne = randomCard->next;
    struct card_llist *pileTwo = *deck;
    *deck = pileOne;
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
    return 0;
}

int insert_shuffle(struct card_llist **deck)
{
    struct card_llist *oldDeck = *deck;
    struct card_llist *insertCard = *deck;
    struct card_llist *newDeck = insertCard;
    oldDeck = oldDeck->next;
    insertCard->next = NULL;

    while (oldDeck != NULL)
    {
        int size = get_cards_size(newDeck);
        int randomIndex = rand() % (size + 1);
        insertCard = oldDeck;
        oldDeck = oldDeck->next;
        insertCard->next = NULL;
        if (randomIndex == 0)
        {
            insertCard->next = newDeck;
            newDeck = insertCard;
        }
        else if (randomIndex == size)
        {
            struct card_llist *lastCard = get_last_card(newDeck);
            lastCard->next = insertCard;
        }
        else
        {
            struct card_llist *randomIndexCard = get_card_by_index(newDeck, randomIndex);
            struct card_llist *tempCard = randomIndexCard->next;
            randomIndexCard->next = insertCard;
            insertCard->next = tempCard;
        }
    }
    *deck = newDeck;
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
    if (lastCard == NULL)
        return -1;
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

int face_value_to_int(char card_value)
{
    switch (card_value)
    {
    case 'A':
        return 1;
    case 'T':
        return 10;
    case 'J':
        return 11;
    case 'Q':
        return 12;
    case 'K':
        return 13;
    default:
        return card_value - '0';
    }
}

char int_to_face_value(int card_value)
{
    switch (card_value)
    {
    case 1:
        return 'A';
    case 10:
        return 'T';
    case 11:
        return 'J';
    case 12:
        return 'Q';
    case 13:
        return 'K';
    default:
        return card_value + '0';
    }
}
