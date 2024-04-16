//
// Created by lemse on 11/04/2024.
//

#include "utils.h"
#include "../model/cardList.h"

int readDeck(char filename[], Card *columns[]){
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return 1;
    }

    // read one character at a time and
    // display it to the output
    char ch[4];
    int value;
    char color;
    int cIdx = 0;
    for(int i = 0;(fgets(ch,4,fp));i++){
        if(i == 7){
            i = 0;
        }
        value = convertCharToInt(ch[0]);
        color = ch[1];
        addCardEnd(createCard(value,color), &columns[i]);
    }
    fclose(fp);
    return 0;
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