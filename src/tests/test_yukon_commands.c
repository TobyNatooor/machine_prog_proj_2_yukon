#include <assert.h>
#include "../main/yukon_commands.h"
#include "test_yukon_commands.h"

int main(void)
{
    test_load_cards();
    return 0;
}

void test_load_cards()
{
    struct card_llist *deck[CARD_COUNT];
    struct card_llist *columns[COLUMNS];
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;

    // Test loading cards without file
    char *response = load_cards(columns, deck, NULL);
    const char cards[CARD_COUNT][CARD_SIZE] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC",
                                               "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD",
                                               "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH",
                                               "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS"};
    assert(strcmp(response, "OK") == 0);

    for (int i = 0; i < CARD_COUNT; i++)
    {
        assert(deck[i] != NULL);
        assert(deck[i]->value == face_value_to_int(cards[i][0]));
        assert(deck[i]->suit == cards[i][1]);
    }

    int j = 0;
    for (int i = 0; i < CARD_COUNT; i++)
    {
        struct card_llist *card;
        while ((card = get_card_by_index(columns[j % COLUMNS], j / COLUMNS)) == NULL)
            j++;
        assert(card->value == face_value_to_int(cards[i][0]));
        assert(card->suit == cards[i][1]);
        j++;
    }

    // Test loading cards with file
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    response = load_cards(columns, deck, "test_load_cards_cards_1.txt");
    assert(strcmp(response, "OK") == 0);
    for (int i = 0; i < CARD_COUNT; i++)
    {
        assert(deck[i] != NULL);
        assert(deck[i]->value == face_value_to_int(cards[i][0]));
        assert(deck[i]->suit == cards[i][1]);
    }

    j = 0;
    for (int i = 0; i < CARD_COUNT; i++)
    {
        struct card_llist *card;
        while ((card = get_card_by_index(columns[j % COLUMNS], j / COLUMNS)) == NULL)
            j++;
        assert(card->value == face_value_to_int(cards[i][0]));
        assert(card->suit == cards[i][1]);
        j++;
    }

    // Test loading cards with invalid file
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    response = load_cards(columns, deck, "test_load_cards_cards_2.txt");
    assert(strcmp(response, "Error loading cards") == 0);
}

void test_show_deck()
{
    struct card_llist *deck[CARD_COUNT];
    struct card_llist *columns[COLUMNS];
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;

    char *response = load_cards(columns, deck, NULL);
    assert(strcmp(response, "OK") == 0);
    for (int i = 0; i < CARD_COUNT; i++)
    {
        assert(deck[i] != NULL);
        assert(deck[i]->hidden == 0);
    }

    response = show_deck(deck);
    assert(strcmp(response, "OK") == 0);
    for (int i = 0; i < CARD_COUNT; i++)
    {
        assert(deck[i] != NULL);
        assert(deck[i]->hidden == 1);
    }
}

void test_shuffle_si() {}

void test_shuffle_sr() {}

void test_save_deck() {}

void test_quit_application() {}

void test_init_play_phase() {}

void test_quit_game() {}

void test_move_cards_from_input() {}

void test_handle_input() {}
