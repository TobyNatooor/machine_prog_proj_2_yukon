#include "../../main/model/card_llist.h"
#include <assert.h>

void linkTwoCardsFindLast(void);
void linkThreeCardsFindLast(void);
void addOneCardToNullPointer(void);
void addTwoCardsToNullPointer(void);

int main(void) {
    linkTwoCardsFindLast();
    linkThreeCardsFindLast();
    addOneCardToNullPointer();
    addTwoCardsToNullPointer();
    return 0;
}

void linkTwoCardsFindLast(void){
    struct card_llist *card1 = (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card2 = (struct card_llist *)malloc(sizeof(struct card_llist));
    int expectedValue = 2;
    char expectedSuit = 'C';
    card1->value = 1;
    card1->suit = 'H';
    card2->value = expectedValue;
    card2->suit = expectedSuit;
    card1->next = card2;
    card2->next = NULL;

    //Testing get_last_card() function
    struct card_llist cardLast = *get_last_card(card1);

    int foundValue = cardLast.value;
    char foundSuit = cardLast.suit;
    assert(foundSuit == expectedSuit && foundValue == expectedValue);
    free(card1);
    free(card2);
}


void linkThreeCardsFindLast(void){
    struct card_llist *card1 = (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card2 = (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card3 = (struct card_llist *)malloc(sizeof(struct card_llist));
    int expectedValue = 3;
    char expectedSuit = 'C';
    card1->value = 1;
    card1->suit = 'H';
    card2->value = 2;
    card2->suit = 'S';
    card3->value = expectedValue;
    card3->suit = expectedSuit;
    card1->next = card2;
    card2->next = card3;
    card3->next = NULL;

    // Testing get_last_card() funtion
    struct card_llist cardLast = *get_last_card(card1);

    int foundValue = cardLast.value;
    char foundSuit = cardLast.suit;
    assert(foundSuit == expectedSuit && foundValue == expectedValue);
    free(card1);
    free(card2);
    free(card3);
}

void addOneCardToNullPointer(void){
    struct card_llist **list = (struct card_llist *)malloc(sizeof(struct card_llist));
    *list = NULL;
    struct card_llist *card = (struct card_llist *)malloc(sizeof(struct card_llist));
    int expectedValue = 3;
    char expectedSuit = 'C';
    card->value = expectedValue;
    card->suit = expectedSuit;
    card->next = NULL;

    // Testing add_card() funtion
    add_card(list, card);

    struct card_llist cardLast = *get_last_card(*list);
    int foundValue = cardLast.value;
    char foundSuit = cardLast.suit;
    assert(foundSuit == expectedSuit && foundValue == expectedValue);
    free(card);
    free(list);
}

void addTwoCardsToNullPointer(void){
    struct card_llist **list = (struct card_llist *)malloc(sizeof(struct card_llist));
    *list = NULL;
    struct card_llist *card1 = (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card2 = (struct card_llist *)malloc(sizeof(struct card_llist));
    int expectedValue = 3;
    char expectedSuit = 'C';
    card1->value = 1;
    card1->suit = 'H';
    card1->next = NULL;
    card2->value = expectedValue;
    card2->suit = expectedSuit;
    card2->next = NULL;

    // Testing add_card() funtion
    add_card(list, card1);
    add_card(list, card2);

    struct card_llist cardLast = *get_last_card(*list);
    int foundValue = cardLast.value;
    char foundSuit = cardLast.suit;
    assert(foundSuit == expectedSuit && foundValue == expectedValue);
    free(card1);
    free(card2);
    free(list);
}

