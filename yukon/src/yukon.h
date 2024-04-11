//
// Created by lemse on 10/04/2024.
//
#ifndef UNTITLED_YUKON_H
#define UNTITLED_YUKON_H

#include <stdbool.h>

typedef struct card Card;

struct card {
    int value;
    char color;
    bool hidden;
    Card *previous;
    Card *next;
};

char heads[4] = {'T','J','Q','K'};

int launch(void);
bool moveCard(Card *newCard, Card **list);
void addCardTop(Card *newCard, Card **list);
void addCardEnd(Card *newCard, Card **list);
Card* createCard(int value, char color);
bool printCard(Card *card);
void printColumn(Card **list);
Card* findCardByIdx(int idx, Card **list);
void printColumns(Card *columns[], Card *foundations[]);
Card* findCardByProp(Card **list, char value, char color);
Card* findLastCard(Card **list);

int convertCharToInt(char charValue){
    int intValue;
    bool inHeads = false;
    for(int i = 0; i < 4; i++){
        if(charValue == heads[i]){
            intValue = i + 10;
            inHeads = true;
        }
    }
    if(charValue == 'A'){
        intValue = 1;
    } else if(!(inHeads)){
        intValue = charValue - '0';
    }
    return intValue;
}

bool isALowerThanB(char A, char B){
    return convertCharToInt(A) < convertCharToInt(B);
}

char convertIntToChar(int intValue){
    char charValue;
    if(intValue >= 10){
        charValue = heads[intValue - 10];
    } else if(intValue == 1){
        charValue = 'A';
    } else {
        charValue = intValue + '0';
    }
    return charValue;
}



#endif //UNTITLED_YUKON_H
