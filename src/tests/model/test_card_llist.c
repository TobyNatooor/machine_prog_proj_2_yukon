#include "../../main/model/card_llist.h"
#include <assert.h>

void linkTwoCardsFindLast(void);
void linkThreeCardsFindLast(void);
void addOneCardToNullPointer(void);
void addTwoCardsToNullPointer(void);
void addThreeSizeShouldBeThree(void);
void getCardByIndex(void);
void addThreeCardsGetCardIdx(void);
void moveCardToNullList(void);
void moveCardMidListCheckFrom(void);
void moveCardShouldSetPointersToNull(void);

int main(void) {
    linkTwoCardsFindLast();
    linkThreeCardsFindLast();
    addOneCardToNullPointer();
    addTwoCardsToNullPointer();
    addThreeSizeShouldBeThree();
    getCardByIndex();
    addThreeCardsGetCardIdx();
    moveCardToNullList();
    moveCardMidListCheckFrom();
    moveCardShouldSetPointersToNull();
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

void addThreeCardsGetCardIdx(void){
    struct card_llist **list = (struct card_llist **)malloc(sizeof(struct card_llist));
    *list = NULL;
    struct card_llist *card1 = (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card2 = (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card3 = (struct card_llist *)malloc(sizeof(struct card_llist));

    int expectedValue1 = 1;
    int expectedValue2 = 2;
    int expectedValue3 = 3;
    char expectedSuit1 = 'H';
    char expectedSuit2 = 'S';
    char expectedSuit3 = 'C';

    card1->value = expectedValue1;
    card1->suit = expectedSuit1;
    card1->next = NULL;
    card2->value = expectedValue2;
    card2->suit = expectedSuit2;
    card2->next = NULL;
    card3->value = expectedValue3;
    card3->suit = expectedSuit3;
    card3->next = NULL;

    add_card(list, card1);
    add_card(list, card2);
    add_card(list, card3);

    int idxCard1 = get_card_index(*list, expectedValue1, expectedSuit1);
    int idxCard2 = get_card_index(*list, expectedValue2, expectedSuit2);
    int idxCard3 = get_card_index(*list, expectedValue3, expectedSuit3);

    assert(idxCard1 == 0 && idxCard2 == 1 && idxCard3 == 2);
}

void moveCardToNullList(void){
    struct card_llist **list1 = (struct card_llist **)malloc(sizeof(struct card_llist));
    struct card_llist **list2 = (struct card_llist **)malloc(sizeof(struct card_llist));
    *list1 = NULL;
    *list2 = NULL;
    struct card_llist *card = (struct card_llist *)malloc(sizeof(struct card_llist));

    int cardValue = 2;
    char cardSuit = 'C';

    card->value = cardValue;
    card->suit = cardSuit;
    card->next = NULL;

    add_card(list1, card);

    int srcIdx = get_card_index(*list1, cardValue, cardSuit);

    //Testing move_cards() function
    move_cards(list1, list2, srcIdx);

    int destIdx = get_card_index(*list2, cardValue, cardSuit);

    assert(destIdx == 0);

    free(list1);
    free(list2);
    free(card);
}

void moveCardMidListCheckFrom(void){
    struct card_llist **list1 = (struct card_llist **)malloc(sizeof(struct card_llist));
    struct card_llist **list2 = (struct card_llist **)malloc(sizeof(struct card_llist));
    *list1 = NULL;
    *list2 = NULL;
    struct card_llist *card1_1= (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card1_2= (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card1_3= (struct card_llist *)malloc(sizeof(struct card_llist));

    int cardValue1 = 1;
    int cardValue2 = 2;
    int cardValue3 = 3;
    char cardSuit1 = 'H';
    char cardSuit2 = 'S';
    char cardSuit3 = 'C';

    card1_1->value = cardValue1;
    card1_1->suit = cardSuit1;
    card1_1->next = NULL;
    card1_2->value = cardValue2;
    card1_2->suit = cardSuit2;
    card1_2->next = NULL;
    card1_3->value = cardValue3;
    card1_3->suit = cardSuit3;
    card1_3->next = NULL;

    add_card(list1, card1_1);
    add_card(list1, card1_2);
    add_card(list1, card1_3);

    move_cards(list1, list2, 1);

    struct card_llist *lastInList1 = get_last_card(*list1);

    int foundValue = lastInList1->value;
    char foundSuit = lastInList1->suit;

    assert(foundValue == cardValue1 && foundSuit == cardSuit1);

    free(card1_1);
    free(card1_2);
    free(card1_3);
    free(list1);
    free(list2);
}

void moveCardShouldSetPointersToNull(void){
    struct card_llist **list1 = (struct card_llist **)malloc(sizeof(struct card_llist));
    struct card_llist **list2 = (struct card_llist **)malloc(sizeof(struct card_llist));
    *list1 = NULL;
    *list2 = NULL;
    struct card_llist *card1_1= (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card1_2= (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card1_3= (struct card_llist *)malloc(sizeof(struct card_llist));

    int cardValue1 = 1;
    int cardValue2 = 2;
    int cardValue3 = 3;
    char cardSuit1 = 'H';
    char cardSuit2 = 'S';
    char cardSuit3 = 'C';

    card1_1->value = cardValue1;
    card1_1->suit = cardSuit1;
    card1_1->next = NULL;
    card1_2->value = cardValue2;
    card1_2->suit = cardSuit2;
    card1_2->next = NULL;
    card1_3->value = cardValue3;
    card1_3->suit = cardSuit3;
    card1_3->next = NULL;

    add_card(list1, card1_1);
    add_card(list1, card1_2);
    add_card(list1, card1_3);

    move_cards(list1, list2, 1);

    struct card_llist *lastInList1 = get_last_card(*list1);

    assert(lastInList1->next == NULL);

    free(card1_1);
    free(card1_2);
    free(card1_3);
    free(list1);
    free(list2);
}
