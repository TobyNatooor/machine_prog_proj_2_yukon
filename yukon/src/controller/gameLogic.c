//
// Created by lemse on 11/04/2024.
//

#include "gameLogic.h"

bool moveToFound(Card *card, Card **foundation){
    printf("\nFunction moveToFound() init");
    Card *current = *foundation;
    printf("\nStart condition");
    if(card->next != NULL){
        printf("\nnext != NULL condition");
        return false;
    }
    printf("\nlet's go");
    if(current == NULL){
        printf("\nCurrent == NULL");
        moveCard(card, foundation);
        return true;
    }
    printf("\n1");
    Card *foundTop = findLastCard(foundation);
    printf("\n2");
    if(foundTop != NULL && !(foundTop->value < card->value && foundTop->color == card->color)){
        return false;
    } else {
        return move(card,foundTop);
    }
}

bool moveCard(Card *newCard, Card **list){
    if(newCard == NULL){
        return false;
    }
    Card *current = *list;
    Card *previous;

    if(current == NULL){
        previous = newCard->previous;
        previous->next = NULL;
        newCard->previous = NULL;
        *list = newCard;
        return true;
    }
    while(current->next != NULL){
        current = current->next;
    }
    if(current->value > newCard->value && current->color != newCard->color){
        previous = newCard->previous;
        newCard->previous = current;
        current->next = newCard;
        previous->next = NULL;
        return true;
    } else return false;
}