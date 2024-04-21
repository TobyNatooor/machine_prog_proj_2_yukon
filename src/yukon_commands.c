#include <string.h>
#include <ctype.h>
#include "yukon_commands.h"

char *load_cards(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT], char input[64])
{
    char *argument = get_argument(input);
    if (argument != NULL)
        return "Error getting input";

    int result;
    if (argument != NULL)
        result = load_cards_from_file(deck, argument);
    else
    {
        const char cards[CARD_COUNT][2] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC",
                                           "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD",
                                           "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH",
                                           "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS"};
        result = load_cards_from_array(deck, cards);
    }
    if (result != 0)
        return "Error loading cards";

    result = deck_to_columns(columns, deck);
    if (result != 0)
        return "Error moving cards to columns";

    return "OK";
}

char *show_deck(struct card_llist *deck[CARD_COUNT])
{
    for (int i = 0; i < CARD_COUNT; i++)
    {
        if (deck[i] == NULL)
            return "Error showing cards";
        deck[i]->hidden = 0;
    }
    return "OK";
}

char *shuffle_si(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT], char input[64])
{
    char *argument = get_argument(input);

    int splitIndex;
    if (argument == NULL)
    {
        splitIndex = rand() % (CARD_COUNT - 1);
    }
    else
    {
        splitIndex = atoi(argument);
        if (splitIndex <= 0 || splitIndex >= CARD_COUNT - 1)
            return "Invalid split index";
    }

    struct card_llist *deck_llist = deck_to_card_llist(deck);
    if (deck_llist == NULL)
        return "Error converting deck to card llist";

    int result = split_shuffle(&deck_llist, splitIndex);
    if (result != 0)
        return "Error shuffling cards";

    for (int i = 0; deck_llist != NULL; i++)
    {
        deck[i] = deck_llist;
        deck_llist = deck_llist->next;
    }
    result = deck_to_columns(columns, deck);
    if (result != 0)
        return "Error moving cards to columns";

    return "OK";
}

char *shuffle_sr(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT])
{
    struct card_llist *deck_llist = deck_to_card_llist(deck);

    int result = insert_shuffle(&deck_llist);
    if (result != 0)
        return "Error shuffling cards";

    for (int i = 0; deck_llist != NULL; i++)
    {
        deck[i] = deck_llist;
        deck_llist = deck_llist->next;
    }
    result = deck_to_columns(columns, deck);
    if (result != 0)
        return "Error moving cards to columns";
    return "OK";
}

char *save_deck(struct card_llist *columns[COLUMNS], char input[64])
{
    char *argument = get_argument(input);

    char save_file_name[64] = "../";
    strcat_s(save_file_name, 64, argument);
    strcat_s(save_file_name, 64, ".txt");
    FILE *save_file = fopen(save_file_name, "w");
    if (save_file == NULL)
        return "Error opening file";

    for (int i = 0; i < COLUMNS; i++)
    {
        struct card_llist *card = columns[i];
        while (card != NULL)
        {
            fprintf(save_file, "%c%c", int_to_face_value(card->value), card->suit);
            card = card->next;
            if (columns[i] != NULL)
                fprintf(save_file, "%c", DELIMITER);
        }
    }
    fclose(save_file);
    return "OK";
}

void quit_application(struct card_llist *columns[COLUMNS], int *playing)
{
    for (int i = 0; i < COLUMNS; i++)
        remove_cards(columns[i]);
    *playing = 0;
}

char *init_play_phase(struct card_llist *columns[COLUMNS], int *inPlayPhase)
{
    if (*inPlayPhase)
        return "Already in PLAY phase";

    *inPlayPhase = 1;
    arrange_cards(columns);
    return "OK";
}

char *quit_game(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT], int *inPlayPhase)
{
    if (!*inPlayPhase)
        return "Can't quit game if not in PLAY phase";
    int result = deck_to_columns(columns, deck);
    if (result != 0)
        return "Error moving cards to columns";
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i]->hidden = 1;
    *inPlayPhase = 0;
    return "OK";
}

char *move_cards_from_input(struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], char input[64])
{
    if (input[0] != 'C')
        return "Invalid command";

    if (input[1] < '1' || input[1] > '7')
        return "Invalid column";

    int value = face_value_to_int(input[3]);
    enum suits suit = input[4];
    struct card_llist **from = &columns[input[1] - '0' - 1];
    int fromIndex = get_card_index(*from, value, suit);
    if (fromIndex == -1)
        return "Card not found";

    int toIndex = input[8] - '0' - 1;
    struct card_llist **to;
    if (input[7] == 'C')
    {
        if (input[8] < '1' || input[8] > '7')
            return "Invalid column number";

        to = &columns[toIndex];
        struct card_llist *lastToCard = get_last_card(*to);
        if (lastToCard->value - 1 != value || lastToCard->suit == suit)
            return "Invalid move";
    }
    else if (input[7] == 'F')
    {
        if (input[8] < '1' || input[8] > '4')
            return "Invalid foundation number";

        to = &foundations[toIndex];
        struct card_llist *lastToCard = get_last_card(*to);
        if (lastToCard == NULL)
        {
            if (value != 1)
                return "Invalid move";
        }
        else if (lastToCard->value != value + 1 || lastToCard->suit != suit)
            return "Invalid move";
    }
    else
        return "Invalid command";

    int result = move_cards(from, to, fromIndex);
    if (result != 0)
        return "Error moving cards";

    if (fromIndex != 0)
    {
        struct card_llist *fromCard = get_card_by_index(*from, fromIndex - 1);
        fromCard->hidden = 0;
    }
    return "OK";
}

char *move_card_from_foundation(struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], char input[64])
{
    if (input[0] != 'F' || input[4] != 'C')
        return "Invalid command";

    if (input[1] < '1' || input[1] > '4')
        return "Invalid foundation number";

    if (input[5] < '1' || input[5] > '7')
        return "Invalid column number";

    struct card_llist **from = &foundations[input[1] - '0' - 1];
    struct card_llist **to = &columns[input[5] - '0' - 1];
    struct card_llist *fromCard = get_last_card(*from);
    if (fromCard == NULL)
        return "No card in foundation";

    struct card_llist *toCard = get_last_card(*to);
    if (toCard == NULL)
    {
        if (fromCard->value != 13)
            return "Invalid move";
    }
    else if (toCard->value != fromCard->value + 1 || toCard->suit == fromCard->suit)
        return "Invalid move";

    int result = move_cards(from, to, get_cards_size(*from) - 1);
    if (result != 0)
        return "Error moving cards";
    return "OK";
}

char *handle_input(struct card_llist *deck[CARD_COUNT], struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], char input[64], int *inPlayPhase, int *playing)
{
    // convert input to uppercase
    char *temp = input;
    for (int i = 0; input[i]; i++)
        input[i] = toupper(input[i]);
    input = temp;

    char *command = get_command(input);
    char *argument = get_argument(input);
    int result;

    if (strcmp(command, "LD") == 0) // Load deck
    {
        if (*inPlayPhase)
            return "Command not available in the PLAY phase";
        return load_cards(columns, deck, argument);
    }
    else if (strcmp(command, "SW") == 0) // Show
    {
        if (*inPlayPhase)
            return "Command not available in the PLAY phase";
        return show_deck(deck);
    }
    else if (strcmp(command, "SI") == 0) // Shuffle, split and interleaves cards
    {
        if (*inPlayPhase)
            return "Command not available in the PLAY phase";
        return shuffle_si(columns, deck, argument);
    }
    else if (strcmp(command, "SR") == 0) // Shuffle, insert randomly into new deck
    {
        if (*inPlayPhase)
            return "Command not available in the PLAY phase";
        return shuffle_sr(columns, deck);
    }
    else if (strcmp(command, "SD") == 0) // Save deck
        return save_deck(columns, argument);
    else if (strcmp(command, "QQ") == 0) // Quit program
        quit_application(columns, playing);
    else if (strcmp(command, "P") == 0) // Play
        return init_play_phase(columns, inPlayPhase);
    else if (strcmp(command, "Q") == 0) // Quit current game
        return quit_game(columns, deck, inPlayPhase);
    else if (strlen(input) == 9 && input[2] == ':' && input[5] == '-' && input[6] == '>') // move card(s)
        return move_cards_from_input(columns, foundations, input);
    else if (strlen(input) == 6 && input[2] == '-' && input[3] == '>') // move card from foundation to column
        return move_card_from_foundation(columns, foundations, input);
    else
        return "Unknown command";
}
