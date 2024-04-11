//
// Created by lemse on 11/04/2024.
//

#ifndef CARDLIST_H
#define CARDLIST_H
#include <stdlib.h>
#include <stdbool.h>

typedef struct card Card;

struct card {
    int value;
    char color;
    bool hidden;
    Card *previous;
    Card *next;
};

Card* createCard(int value, char color);
Card* findCardByIdx(int idx, Card **list);
Card* findCardByProp(Card **list, char value, char color);
Card* findLastCard(Card **list);
void addCardEnd(Card *newCard, Card **list);

#endif //CARDLIST_H
