//
// Created by lemse on 05/04/2024.
//

#include <stdio.h>
#include "yukon.h"


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

int read(char filename[], Card **list){
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return 1;
    }

    // read one character at a time and
    // display it to the output
    char ch[4];
    Card *current;
    int value;
    char color;
    fgets(ch,4,fp);
    value = convertCharToInt(ch[0]);
    color = ch[1];
    Card *first = createCard(value, color);
    current = first;
    Card next;
    while ((fgets(ch,4,fp))) {
        value = convertCharToInt(ch[0]);
        color = ch[1];
        current->next = createCard(value, color);
        current = current->next;
    }
    // close the file
    fclose(fp);
    *list = first;
    printf("File has been read and stored\n");

    return 0;
}