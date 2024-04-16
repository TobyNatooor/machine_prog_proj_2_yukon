#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "yukon.h"

#define DELIMITER '\n'

struct card_llist *load_deck_from_file(char *fileName)
{
    struct card_llist *deck = NULL;
    FILE *cards_file = fopen(fileName, "r");
    // if 'cards.txt' isn't found, try in the parent directory
    if (cards_file == NULL)
    {
        char path[32] = "../";
        strcat(path, fileName);
        cards_file = fopen(path, "r");
    }
    if (cards_file == NULL)
        return NULL;
    char character, faceValue, suit;

    for (int i = 0; (character = fgetc(cards_file)) != EOF; ++i)
    {
        if (character == DELIMITER)
        {
            // create card
            struct card_llist *card = (struct card_llist *)malloc(sizeof(struct card_llist));
            card->value = face_value_to_int(faceValue);
            card->suit = suit;
            card->hidden = 1;
            card->next = NULL;

            add_card(&deck, card);

            i = -1;
            continue;
        }

        if (i == 0)
            faceValue = character;
        else if (i == 1)
            suit = character;
        else if (i == 2)
            return NULL;
    }
    fclose(cards_file);

    return deck;
}

struct card_llist *load_cards_from_array(const char cards[CARD_COUNT][CARD_SIZE])
{
    struct card_llist *deck = NULL;
    for (int i = 0; i < CARD_COUNT; i++)
    {
        // create card
        struct card_llist *card = (struct card_llist *)malloc(sizeof(struct card_llist));
        card->value = face_value_to_int(cards[i][0]);
        card->suit = cards[i][1];
        card->hidden = 1;
        card->next = NULL;

        add_card(&deck, card);
    }
    return deck;
}

int arrange_cards(struct card_llist *columns[COLUMNS])
{
    const int columnSizes[COLUMNS] = {1, 6, 7, 8, 9, 10, 11};
    const int columnsIndexShown[COLUMNS] = {0, 1, 2, 3, 4, 5, 6};

    for (int i = 0; i < COLUMNS - 1; i++)
        move_cards(&columns[i], &columns[i + 1], columnSizes[i]);
    for (int i = 0; i < COLUMNS; i++)
        show_after_index(columns[i], columnsIndexShown[i]);

    return 0;
}

int display_game(struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS])
{
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    int colsDone;
    // print rows until all columns are done and all rows are printed
    for (int row = 0; colsDone != COLUMNS || row < 7; row++)
    {
        // print columns
        colsDone = 0;
        for (int j = 0; j < COLUMNS; j++)
        {
            struct card_llist *nextCard = get_card_by_index(columns[j], row);
            if (nextCard == NULL)
            {
                colsDone++;
                printf("\t");
            }
            else
            {
                if (nextCard->hidden)
                    printf("[]\t");
                else
                    printf("%c%c\t", int_to_face_value(nextCard->value), nextCard->suit);
            }
        }

        // foundations
        if (row == 0 || row == 2 || row == 4 || row == 6)
        {
            int foundationNr = row / 2 + 1;
            if (foundations[foundationNr - 1] == NULL)
            {
                printf("\t[]\tF%d", foundationNr);
            }
            else
            {
                struct card_llist *foundationCard = get_last_card(foundations[foundationNr - 1]);
                char foundationValue = int_to_face_value(foundationCard->value);
                printf("\t%c%c\tF%d", foundationValue, foundationCard->suit, foundationNr);
            }
        }
        printf("\n");
    }
    return 0;
}

int face_value_to_int(char card_value)
{
    switch (card_value)
    {
    case 'A':
        return 1;
    case 'T':
        return 10;
    case 'J':
        return 11;
    case 'Q':
        return 12;
    case 'K':
        return 13;
    default:
        return card_value - '0';
    }
}

char int_to_face_value(int card_value)
{
    switch (card_value)
    {
    case 1:
        return 'A';
    case 10:
        return 'T';
    case 11:
        return 'J';
    case 12:
        return 'Q';
    case 13:
        return 'K';
    default:
        return card_value + '0';
    }
}

int print_deck(struct card_llist *deck)
{
    struct card_llist *card = deck;
    while (card != NULL)
    {
        printf("%c%c ", int_to_face_value(card->value), card->suit);
        card = card->next;
    }
    printf("\n");
    return 0;
}

int print_columns(struct card_llist *column[COLUMNS])
{
    for (int i = 0; i < COLUMNS; i++)
    {
        printf("Column %d: ", i);
        print_deck(column[i]);
        printf("\n");
    }
    return 0;
}

int deck_to_columns(struct card_llist *columns[COLUMNS], struct card_llist *deck)
{
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;

    for (int i = 0; deck != NULL; i++)
    {
        i = i % COLUMNS;
        struct card_llist *addCard = deck;
        deck = deck->next;
        addCard->next = NULL;
        add_card(&columns[i], addCard);
    }
    return 0;
}

struct card_llist *columns_to_deck(struct card_llist *columns[COLUMNS])
{
    struct card_llist *deck = columns[0];
    for (int i = 0; i < COLUMNS - 1; i++)
    {
        struct card_llist *lastCard = get_last_card(columns[i]);
        lastCard->next = columns[i + 1];
    }
    return deck;
}

int show_after_index(struct card_llist *column, int index)
{
    struct card_llist *card = column;
    for (int i = 0; card != NULL; i++)
    {
        if (i < index)
            card->hidden = 1;
        else
            card->hidden = 0;
        card = card->next;
    }
    return 0;
}

int start_game()
{
    // initialize game
    struct card_llist *columns[COLUMNS];
    struct card_llist *foundations[FOUNDATIONS];
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    for (int i = 0; i < FOUNDATIONS; i++)
        foundations[i] = NULL;

    // game loop
    int result;
    int playing = 1;
    int inPlayPhase = 0;
    char input[64];
    char inputArg[64];
    char message[64] = "";
    char lastCommand[64] = "";
    while (playing)
    {
        result = display_game(columns, foundations);
        if (result != 0)
            strcpy(message, "Error displaying game");

        printf("LAST Command: ");
        printf("%s", lastCommand);
        printf("\nMessage: ");
        printf("%s", message);
        printf("\nINPUT > ");
        fgets(input, 64, stdin);
        input[strcspn(input, "\n")] = 0; // remove newline

        strcpy(lastCommand, input);

        char *token = strtok(input, " ");
        if (token == NULL)
        {
            strcpy(message, "No command entered");
            continue;
        }
        strcpy(input, token);
        token = strtok(NULL, " ");
        if (token != NULL)
        {
            if (strtok(NULL, " ") != NULL)
            {
                strcpy(message, "Too many arguments");
                continue;
            }
            strcpy(inputArg, token);
        }
        else
        {
            strcpy(inputArg, "");
        }

        strcpy(message, "OK");
        if (strcmp(input, "ld") == 0 || strcmp(input, "LD") == 0) // Load deck
        {
            if (inPlayPhase)
            {
                strcpy(message, "Command not available in the PLAY phase");
                continue;
            }
            struct card_llist *deck;

            if (strcmp(inputArg, "") != 0)
                deck = load_deck_from_file(inputArg);
            else
            {
                const char cards[CARD_COUNT][2] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC",
                                                   "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD",
                                                   "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH",
                                                   "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS"};
                deck = load_cards_from_array(cards);
            }

            if (deck == NULL || get_cards_size(deck) != CARD_COUNT)
            {
                strcpy(message, "Error loading cards");
                continue;
            }

            result = deck_to_columns(columns, deck);
            if (result != 0)
            {
                strcpy(message, "Error moving cards to columns");
                continue;
            }
        }
        else if (strcmp(input, "sw") == 0 || strcmp(input, "SW") == 0) // Show
        {
            if (inPlayPhase)
            {
                strcpy(message, "Command not available in the PLAY phase");
                continue;
            }
            for (int i = 0; i < COLUMNS; i++)
            {
                result = show_after_index(columns[i], 0);
                if (result != 0)
                {
                    strcpy(message, "Error showing cards");
                    continue;
                }
            }
        }
        else if (strcmp(input, "si") == 0 || strcmp(input, "SI") == 0) // Shuffle, split and interleaves cards
        {
            if (inPlayPhase)
            {
                strcpy(message, "Command not available in the PLAY phase");
                continue;
            }
            int splitIndex;
            if (strcmp(inputArg, "") == 0)
            {
                splitIndex = rand() % (CARD_COUNT - 1);
            }
            else
            {
                splitIndex = atoi(inputArg);
                if (splitIndex <= 0 || splitIndex >= CARD_COUNT - 1)
                {
                    strcpy(message, "Invalid split index");
                    continue;
                }
            }

            struct card_llist *deck = columns_to_deck(columns);
            if (deck == NULL)
            {
                strcpy(message, "Error moving cards to deck");
                continue;
            }

            result = split_shuffle(&deck, splitIndex);
            if (result != 0)
            {
                strcpy(message, "Error shuffling cards");
                continue;
            }

            result = deck_to_columns(columns, deck);
            if (result != 0)
            {
                strcpy(message, "Error moving cards to columns");
                continue;
            }
        }
        else if (strcmp(input, "sr") == 0 || strcmp(input, "SR") == 0) // Shuffle, insert randomly into new deck
        {
            if (inPlayPhase)
            {
                strcpy(message, "Command not available in the PLAY phase");
                continue;
            }
            struct card_llist *deck = columns_to_deck(columns);
            result = insert_shuffle(&deck);
            if (result != 0)
            {
                strcpy(message, "Error shuffling cards");
                continue;
            }
            result = deck_to_columns(columns, deck);
            if (result != 0)
            {
                strcpy(message, "Error moving cards to columns");
                continue;
            }
        }
        else if (strcmp(input, "sd") == 0 || strcmp(input, "SD") == 0) // Save deck
        {
            char save_file_name[64] = "../";
            strcat_s(save_file_name, 64, inputArg);
            strcat_s(save_file_name, 64, ".txt");
            FILE *save_file = fopen(save_file_name, "w");
            if (save_file == NULL)
            {
                strcpy(message, "Error opening file");
                continue;
            }
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
        }
        else if (strcmp(input, "qq") == 0 || strcmp(input, "QQ") == 0) // Quit program
        {
            for (int i = 0; i < COLUMNS; i++)
                remove_cards(columns[i]);
            playing = 0;
        }
        else if (strcmp(input, "p") == 0 || strcmp(input, "P") == 0) // Play
        {
            if (inPlayPhase)
            {
                strcpy(message, "Already in PLAY phase");
                continue;
            }
            inPlayPhase = 1;
            arrange_cards(columns);
        }
        else if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0) // Quit current game
        {
        }
        else if (strlen(input) == 9 && input[2] == ':' && input[5] == '-' && input[6] == '>') // move card(s)
        {
            if (input[0] != 'C')
            {
                strcpy(message, "Invalid command");
                continue;
            }
            if (input[1] < '1' || input[1] > '7')
            {
                strcpy(message, "Invalid column");
                continue;
            }

            int value = face_value_to_int(input[3]);
            enum suits suit = input[4];
            struct card_llist **from = &columns[input[1] - '0' - 1];
            int fromIndex = get_card_index(*from, value, suit);
            if (fromIndex == -1)
            {
                strcpy(message, "Card not found");
                continue;
            }
            int toIndex = input[8] - '0' - 1;
            struct card_llist **to;
            if (input[7] == 'C')
            {
                if (input[8] < '1' || input[8] > '7')
                {
                    strcpy(message, "Invalid column number");
                    continue;
                }
                to = &columns[toIndex];
                struct card_llist *lastToCard = get_last_card(*to);
                if (lastToCard->value - 1 != value || lastToCard->suit == suit)
                {
                    strcpy(message, "Invalid move");
                    continue;
                }
            }
            else if (input[7] == 'F')
            {
                if (input[8] < '1' || input[8] > '4')
                {
                    strcpy(message, "Invalid foundation number");
                    continue;
                }
                to = &foundations[toIndex];
                struct card_llist *lastToCard = get_last_card(*to);
                if (lastToCard == NULL)
                {
                    if (value != 1)
                    {
                        strcpy(message, "Invalid move");
                        continue;
                    }
                }
                else if (lastToCard->value != value + 1 || lastToCard->suit != suit)
                {
                    strcpy(message, "Invalid move");
                    continue;
                }
            }
            else
            {
                strcpy(message, "Invalid command");
                continue;
            }
            result = move_cards(from, to, fromIndex);
            if (result != 0)
            {
                strcpy(message, "Error moving cards");
                continue;
            }
            if (fromIndex != 0)
            {
                struct card_llist *fromCard = get_card_by_index(*from, fromIndex - 1);
                fromCard->hidden = 0;
            }
        }
        else if (strlen(input) == 6 && input[2] == '-' && input[3] == '>') // move card from foundation to column
        {
            if (input[0] != 'F' || input[4] != 'C')
            {
                strcpy(message, "Invalid command");
                continue;
            }
            if (input[1] < '1' || input[1] > '4')
            {
                strcpy(message, "Invalid foundation number");
                continue;
            }
            if (input[5] < '1' || input[5] > '7')
            {
                strcpy(message, "Invalid column number");
                continue;
            }
            struct card_llist **from = &foundations[input[1] - '0' - 1];
            struct card_llist **to = &columns[input[5] - '0' - 1];
            struct card_llist *fromCard = get_last_card(*from);
            if (fromCard == NULL)
            {
                strcpy(message, "No card in foundation");
                continue;
            }
            struct card_llist *toCard = get_last_card(*to);
            if (toCard == NULL)
            {
                if (fromCard->value != 13)
                {
                    strcpy(message, "Invalid move");
                    continue;
                }
            }
            else if (toCard->value != fromCard->value + 1 || toCard->suit == fromCard->suit)
            {
                strcpy(message, "Invalid move");
                continue;
            }
            result = move_cards(from, to, get_cards_size(*from) - 1);
            if (result != 0)
            {
                strcpy(message, "Error moving cards");
                continue;
            }
        }
        else
        {
            strcpy(message, "Unknown command");
        }
        int allColumnsEmpty = 1;
        for (int i = 0; i < COLUMNS; i++)
        {
            if (columns[i] != NULL)
            {
                allColumnsEmpty = 0;
                break;
            }
        }
        if (allColumnsEmpty)
        {
            inPlayPhase = 0;
            printf("You won!\n");
        }
    }
}
