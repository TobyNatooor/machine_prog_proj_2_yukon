//
// Created by lemse on 11/04/2024.
//

#include "gameLogic.h"
#include "../cardList/cardList.h"


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