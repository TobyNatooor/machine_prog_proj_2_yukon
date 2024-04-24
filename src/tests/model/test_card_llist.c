#include "../../main/model/card_llist.h"
#include <assert.h>

void test1(void);
void test2(void);

int main(void) {
    test1();
    test2();
    return 0;
}

void test1(void){
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

    struct card_llist cardLast = *get_last_card(card1);
    int foundValue = cardLast.value;
    char foundSuit = cardLast.suit;
    assert(foundSuit == expectedSuit && foundValue == expectedValue);
    free(card1);
    free(card2);
}

void test2(void){
    struct card_llist *card1 = (struct card_llist *)malloc(sizeof(struct card_llist));
    struct card_llist *card2 = (struct card_llist *)malloc(sizeof(struct card_llist));
    int expectedValue = 2;
    char expectedSuit = 'C';
    card1->value = 1;
    card1->suit = 'H';
    card2->value = 3;
    card2->suit = expectedSuit;
    card1->next = card2;
    card2->next = NULL;

    struct card_llist cardLast = *get_last_card(card1);
    int foundValue = cardLast.value;
    char foundSuit = cardLast.suit;
    assert(foundSuit == expectedSuit && foundValue == expectedValue);
    free(card1);
    free(card2);
}
