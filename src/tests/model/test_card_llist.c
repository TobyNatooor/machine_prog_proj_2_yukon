#include "../../main/model/card_llist.h"
#include <assert.h>

void linkTwoCardsFindLast(void);
void linkThreeCardsFindLast(void);
void addOneCardToNullPointer(void);
void addTwoCardsToNullPointer(void);
void addThreeSizeShouldBeThree(void);
void getCardByIndex(void);

int main(void) {
    linkTwoCardsFindLast();
    linkThreeCardsFindLast();
    addOneCardToNullPointer();
    addTwoCardsToNullPointer();
    addThreeSizeShouldBeThree();
    getCardByIndex();
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
    struct card_llist **list = (struct card_llist **)malloc(sizeof(struct card_llist));
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
    struct card_llist **list = (struct card_llist **)malloc(sizeof(struct card_llist));
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

void addThreeSizeShouldBeThree(){
    struct card_llist **list = (struct card_llist **)malloc(sizeof(struct card_llist));
    *list = NULL;
    struct card_llist *card1 = (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card2 = (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card3 = (struct card_llist *)malloc(sizeof(struct card_llist));
    card1->value = 1;
    card1->suit = 'H';
    card1->next = NULL;
    card2->value = 2;
    card2->suit = 'S';
    card2->next = NULL;
    card3->value = 3;
    card3->suit = 'C';
    card3->next = NULL;

    add_card(list, card1);
    add_card(list, card2);
    add_card(list, card3);

    //Testing get_cards_size() function
    int size = get_cards_size(*list);
    assert(size == 3);
    free(card1);
    free(card2);
    free(card3);
    free(list);
}

void getCardByIndex(void){
    struct card_llist **list = (struct card_llist **)malloc(sizeof(struct card_llist));
    *list = NULL;
    struct card_llist *card1 = (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card2 = (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card3 = (struct card_llist *)malloc(sizeof(struct card_llist));
    int expectedValue = 2;
    char expectedSuit = 'C';
    card1->value = 1;
    card1->suit = 'H';
    card1->next = NULL;
    card2->value = expectedValue;
    card2->suit = expectedSuit;
    card2->next = NULL;
    card3->value = 3;
    card3->suit = 'S';
    card3->next = NULL;

    add_card(list, card1);
    add_card(list, card2);
    add_card(list, card3);

    int idx = 1;

    struct card_llist cardAtIdx = *get_card_by_index(*list, idx);
    int foundValue = cardAtIdx.value;
    char foundSuit = cardAtIdx.suit;
    assert(foundSuit == expectedSuit && foundValue == expectedValue);
    free(card1);
    free(card2);
    free(card3);
    free(list);
}

void moveCardShouldSetPointersToNull(void){
    
}


