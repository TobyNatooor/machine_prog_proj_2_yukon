//
// Created by lemse on 11/04/2024.
//

#include "cardList.h"
#include "../utils/utils.h"

Card* createCard(int value, char color){
    Card *newCard;
    newCard = (Card*)malloc(sizeof(Card));

    newCard->value = value;
    newCard->color = color;
    newCard->hidden = false;
    newCard->previous = NULL;
    newCard->next = NULL;
    return newCard;
}

Card* findCardByProp(Card **list, char value, char color){
    Card *current = *list;
    if(current == NULL){
        return NULL;
    }
    int fromValue = convertCharToInt(value);
    while(current->next != NULL){
        //printf("Comparing %d%c with %d%c\n",current->value, current->color, fromValue, color);
        if(current->value == fromValue && current->color == color){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void addCardEnd(Card *newCard, Card **list){
    Card *current = *list;

    if(current == NULL){
        *list = newCard;
        return;
    }
    while(current->next != NULL){
        current = current->next;
    }
    current->next = newCard;
    newCard->previous = current;
}

Card* findCardByIdx(int idx, Card **list){
    Card *current = *list;
    if(current == NULL){
        return current;
    }
    for(int i = 1; i < idx; i++){
        if(current->next == NULL){
            return NULL;
        }
        current = current->next;
    }
    return current;
}

Card* findLastCard(Card **list){
    Card* current = *list;
    if(current == NULL){
        return NULL;
    }
    while (current->next != NULL){
        current = current->next;
    }
    return current;
}

bool move(Card *srcCard, Card *dstCard){
    if(dstCard->next != NULL){
        return false;
    }
    Card *previous;
    dstCard->next = srcCard;
    if(srcCard->previous == NULL){
        srcCard->previous = dstCard;
    } else{
        previous = srcCard->previous;
        srcCard->previous = dstCard;
        previous->next = NULL;
    }
    return true;
}
