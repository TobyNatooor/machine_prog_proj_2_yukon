//
// Created by lemse on 11/04/2024.
//

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdio.h>


char heads[4] = {'T','J','Q','K'};

int readDeck(char filename[], Card *columns[]);
bool printCard(Card *card);
void printColumn(Card **list);
void printColumns(Card *columns[], Card *foundations[]);
int convertCharToInt(char charValue);
char convertIntToChar(int intValue);

#endif //UTILS_H
