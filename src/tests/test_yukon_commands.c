#include <assert.h>
#include "../main/yukon_commands.h"
#include "test_yukon_commands.h"

int main(void)
{
    test_load_cards();
    test_show_deck();
    test_shuffle_si();
    test_shuffle_sr();
    test_save_deck();
    test_quit_application();
    test_init_play_phase();
    test_quit_game();
    test_move_cards_from_input();
    test_handle_input();
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
        assert(deck[i]->hidden == 1);
    }

    response = show_deck(deck);
    assert(strcmp(response, "OK") == 0);
    for (int i = 0; i < CARD_COUNT; i++)
    {
        assert(deck[i] != NULL);
        assert(deck[i]->hidden == 0);
    }
}

void test_shuffle_si() {}

void test_shuffle_sr() {}

void test_save_deck()
{
    struct card_llist *deck[CARD_COUNT];
    struct card_llist *columns[COLUMNS];
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;

    char *response = load_cards(columns, deck, NULL);
    assert(strcmp(response, "OK") == 0);

    response = save_deck(deck, "test_save_deck_deck_1.txt");
    assert(strcmp(response, "OK") == 0);

    const char cards[CARD_COUNT][CARD_SIZE] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC",
                                               "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD",
                                               "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH",
                                               "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS"};
    FILE *saved_file = fopen("test_save_deck_deck_1.txt", "r");
    assert(saved_file != NULL);
    for (int i = 0; i < CARD_COUNT; i++)
    {
        char character;
        for (int j = 0; (character = fgetc(saved_file)) != '\n'; j++)
            if (j == 0)
                assert(character == cards[i][0]);
            else if (j == 1)
                assert(character == cards[i][1]);
            else
                break;
    }
    fclose(saved_file);
}

void test_quit_application()
{
    struct card_llist *deck[CARD_COUNT];
    struct card_llist *columns[COLUMNS];
    struct card_llist *foundations[FOUNDATIONS];
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    for (int i = 0; i < FOUNDATIONS; i++)
        foundations[i] = NULL;
    int playing = 1;
    char *response = load_cards(columns, deck, NULL);
    assert(strcmp(response, "OK") == 0);
    quit_application(foundations, columns, &playing);
    assert(playing == 0);
}

void test_init_play_phase()
{
    struct card_llist *deck[CARD_COUNT];
    struct card_llist *columns[COLUMNS];
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    int inPlayPhase = 0;
    char *response = load_cards(columns, deck, NULL);
    assert(strcmp(response, "OK") == 0);
    response = init_play_phase(columns, deck, &inPlayPhase);
    assert(strcmp(response, "OK") == 0);
    const int columnSizes[COLUMNS] = {1, 6, 7, 8, 9, 10, 11};
    for (int i = 0; i < COLUMNS; i++)
        assert(get_cards_size(columns[i]) == columnSizes[i]);
    response = init_play_phase(columns, deck, &inPlayPhase);
    assert(strcmp(response, "Already in PLAY phase") == 0);
}

void test_quit_game()
{
    struct card_llist *deck[CARD_COUNT];
    struct card_llist *columns[COLUMNS];
    struct card_llist *foundations[FOUNDATIONS];
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    for (int i = 0; i < FOUNDATIONS; i++)
        foundations[i] = NULL;
    int inPlayPhase = 1;
    char *response = load_cards(columns, deck, NULL);
    assert(strcmp(response, "OK") == 0);
    response = quit_game(foundations, columns, deck, &inPlayPhase);
    assert(strcmp(response, "OK") == 0);
    for (int i = 0; i < CARD_COUNT; i++)
        assert(deck[i]->hidden == 1);
    for (int i = 0; i < FOUNDATIONS; i++)
        assert(foundations[i] == NULL);
    assert(inPlayPhase == 0);
    response = quit_game(foundations, columns, deck, &inPlayPhase);
    assert(strcmp(response, "Can't quit game if not in PLAY phase") == 0);
}

void test_move_cards_from_input()
{
    struct card_llist *deck[CARD_COUNT];
    struct card_llist *columns[COLUMNS];
    struct card_llist *foundations[FOUNDATIONS];
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    for (int i = 0; i < FOUNDATIONS; i++)
        foundations[i] = NULL;
    int inPlayPhase = 0;

    char *response = load_cards(columns, deck, NULL);
    assert(strcmp(response, "OK") == 0);
    response = init_play_phase(columns, deck, &inPlayPhase);
    assert(strcmp(response, "OK") == 0);
    response = move_cards_from_input(columns, foundations, "C8:0->F1");
    assert(strcmp(response, "Invalid column number") == 0);
    response = move_cards_from_input(columns, foundations, "F5:0->C5");
    assert(strcmp(response, "Invalid foundation number") == 0);
    response = move_cards_from_input(columns, foundations, "X1:0->F1");
    assert(strcmp(response, "Invalid command") == 0);
    response = move_cards_from_input(columns, foundations, "C1->F1");
    assert(strcmp(response, "OK") == 0);
    assert(columns[0] == NULL);
    assert(foundations[0] != NULL);
    assert(foundations[0]->value == 1);
    assert(foundations[0]->suit == 'C');
}

void test_handle_input()
{
    struct card_llist *deck[CARD_COUNT];
    struct card_llist *columns[COLUMNS];
    struct card_llist *foundations[FOUNDATIONS];
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    for (int i = 0; i < FOUNDATIONS; i++)
        foundations[i] = NULL;

    int playing = 1;
    int inPlayPhase = 0;
    char *input = malloc(64 * sizeof(char));
    input[0] = '\0';
    char *message = malloc(64 * sizeof(char));
    message[0] = '\0';

    char *response = handle_input(deck, columns, foundations, "", &inPlayPhase, &playing);
    assert(strcmp(response, "No input provided") == 0);
    response = handle_input(deck, columns, foundations, "xxx", &inPlayPhase, &playing);
    assert(strcmp(response, "Unknown command") == 0);
    

    free(input);
    free(message);
}
