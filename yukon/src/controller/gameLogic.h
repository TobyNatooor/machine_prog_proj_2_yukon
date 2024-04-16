//
// Created by lemse on 11/04/2024.
//

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <stdbool.h>
#include "../model/cardList.h"

bool moveCard(Card *newCard, Card **list);
bool moveToFound(Card *card, Card **foundation);

#endif //GAMELOGIC_H
