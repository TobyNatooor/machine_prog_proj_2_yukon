#include "yukon_commands.h"

char *load_cards(struct card_llist *columns[COLUMNS], char input[64])
{
    char inputArg[64];
    int result = get_input(input, inputArg);
    struct card_llist *deck;

    if (strcmp(inputArg, "") != 0)
        deck = load_cards_from_file(inputArg);
    else
    {
        const char cards[CARD_COUNT][2] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC",
                                           "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD",
                                           "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH",
                                           "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS"};
        deck = load_cards_from_array(cards);
    }

    if (deck == NULL || get_cards_size(deck) != CARD_COUNT)
        return "Error loading cards";

    result = deck_to_columns(columns, deck);
    if (result != 0)
        return "Error moving cards to columns";
    return "OK";
}

char *shuffle_si(struct card_llist *columns[COLUMNS], char input[64])
{
    char inputArg[64];
    int result = get_input(input, inputArg);

    int splitIndex;
    if (strcmp(inputArg, "") == 0)
    {
        splitIndex = rand() % (CARD_COUNT - 1);
    }
    else
    {
        splitIndex = atoi(inputArg);
        if (splitIndex <= 0 || splitIndex >= CARD_COUNT - 1)
            return "Invalid split index";
    }

    struct card_llist *deck = columns_to_deck(columns);
    if (deck == NULL)
        return "Error moving cards to deck";

    result = split_shuffle(&deck, splitIndex);
    if (result != 0)
        return "Error shuffling cards";

    result = deck_to_columns(columns, deck);
    if (result != 0)
        return "Error moving cards to columns";
    return "OK";
}

char *save_deck(struct card_llist *columns[COLUMNS], char input[64])
{
    char inputArg[64];
    int result = get_input(input, inputArg);

    char save_file_name[64] = "../";
    strcat_s(save_file_name, 64, inputArg);
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

char *show_deck(struct card_llist *columns[COLUMNS])
{
    for (int i = 0; i < COLUMNS; i++)
    {
        int result = show_after_index(columns[i], 0);
        if (result != 0)
            return "Error showing cards";
    }
    return "OK";
}

char *shuffle_sr(struct card_llist *columns[COLUMNS])
{
    struct card_llist *deck = columns_to_deck(columns);
    int result = insert_shuffle(&deck);
    if (result != 0)
        return "Error shuffling cards";

    result = deck_to_columns(columns, deck);
    if (result != 0)
        return "Error moving cards to columns";
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

char *handle_input(struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], char input[64], int *inPlayPhase, int *playing)
{
    int result;

    if (strcmp(input, "ld") == 0 || strcmp(input, "LD") == 0) // Load deck
    {
        if (*inPlayPhase)
            return "Command not available in the PLAY phase";
        return load_cards(columns, input);
    }
    else if (strcmp(input, "sw") == 0 || strcmp(input, "SW") == 0) // Show
    {
        if (*inPlayPhase)
            return "Command not available in the PLAY phase";
        return show_deck(columns);
    }
    else if (strcmp(input, "si") == 0 || strcmp(input, "SI") == 0) // Shuffle, split and interleaves cards
    {
        if (*inPlayPhase)
            return "Command not available in the PLAY phase";
        return shuffle_si(columns, input);
    }
    else if (strcmp(input, "sr") == 0 || strcmp(input, "SR") == 0) // Shuffle, insert randomly into new deck
    {
        if (*inPlayPhase)
            return "Command not available in the PLAY phase";
        return shuffle_sr(columns);
    }
    else if (strcmp(input, "sd") == 0 || strcmp(input, "SD") == 0) // Save deck
    {
        return save_deck(columns, input);
    }
    else if (strcmp(input, "qq") == 0 || strcmp(input, "QQ") == 0) // Quit program
    {
        for (int i = 0; i < COLUMNS; i++)
            remove_cards(columns[i]);
        *playing = 0;
    }
    else if (strcmp(input, "p") == 0 || strcmp(input, "P") == 0) // Play
    {
        if (*inPlayPhase)
            return "Already in PLAY phase";

        *inPlayPhase = 1;
        arrange_cards(columns);
    }
    else if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0) // Quit current game
    {
    }
    else if (strlen(input) == 9 && input[2] == ':' && input[5] == '-' && input[6] == '>') // move card(s)
    {
        return move_cards_from_input(columns, foundations, input);
    }
    else if (strlen(input) == 6 && input[2] == '-' && input[3] == '>') // move card from foundation to column
    {
        return move_card_from_foundation(columns, foundations, input);
    }
    else
        return "Unknown command";

    return "OK";
}
