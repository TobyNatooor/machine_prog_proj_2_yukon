//
// Created by lemse on 22/03/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "reader.c"
//#include "yukon.h"

int main(void){
    int numberOfColumns = 7;
    int numberOfFoundations = 4;
    Card *columns[numberOfColumns];
    Card *foundations[numberOfFoundations];

    for(int i = 0; i < numberOfColumns; i++){
        columns[i] = NULL;
    }
    for(int i = 0; i < numberOfFoundations; i++){
        foundations[i] = NULL;
    }

    readDeck("unshuffled.txt", columns);

    printColumns(columns, foundations);

    int srcColumn = 7;
    int dstColumn = 6;
    //moveCard(createCard(2,'H'),&columns[dstColumn-1]);
    //printColumns(columns);
    moveCard(findCardByProp(&columns[srcColumn-1],'8','D'), &columns[dstColumn-1]);
    printColumns(columns, foundations);
}

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

void addCardTop(Card *newCard, Card **list){
    Card *top = *list;

    if(top == NULL){
        *list = newCard;
        return;
    } else {
        newCard->next = top;
        *list = newCard;
    }
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

void printColumn(Card **list){
    Card *current = *list;
    if(current == NULL){
        printf("Column is empty\n");
        return;
    }

    while (current->next != NULL){
        printCard(current);
        printf("\n");
        current = current->next;
    }
    printCard(current);
}

bool printCard(Card *card){
    bool isNotNull;
    char value;
    char color;
    if(card == NULL){
        printf("     ");
        isNotNull = false;
    } else if(card->hidden){
        printf("[]   ");
        isNotNull = true;
    } else{
        value = convertIntToChar(card->value);
        color = card->color;
        printf("%c%c   ", value, color);
        isNotNull = true;
    }
    return isNotNull;

}

void printColumns(Card *columns[], Card *foundations[]){
    bool moreCards = true;
    Card *toPrint;
    int idx = 1;
    for(int i = 0; i < 7; i++){
        printf("C%d   ", (i+1));
    }

    printf("\n");
    printf("\n");

    int fIdx = 0;

    while(moreCards){
        moreCards = false;
        for(int i = 0; i < 7; i++){
            toPrint = findCardByIdx(idx, &columns[(i)]);
            if(printCard(toPrint) || idx <= 7){
                moreCards = true;
            }
        }
        if(idx % 2 == 1 && fIdx < 4){
            printf("      ");
            toPrint = findLastCard(&foundations[fIdx]);
            if(toPrint == NULL) printf("[]   ");
            else printCard(toPrint);
            printf("F%d", (fIdx+1));
            fIdx++;
        }
        printf("\n");
        idx++;
    }
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