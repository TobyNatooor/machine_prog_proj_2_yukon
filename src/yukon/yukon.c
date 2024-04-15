#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "yukon.h"

#define DELIMITER '\n'

#ifdef FILE_NAME
struct card_llist *load_deck_from_file()
{
    struct card_llist *deck = NULL;
    FILE *cards_file = fopen(FILE_NAME, "r");
    // if 'cards.txt' isn't found, try in the parent directory
    if (cards_file == NULL)
    {
        char path[32] = "../";
        strcat(path, FILE_NAME);
        cards_file = fopen(path, "r");
    }
    if (cards_file == NULL)
    {
        printf("Error opening file\n");
        return NULL;
    }
    char character, face_value, suit;

    for (int i = 0; (character = fgetc(cards_file)) != EOF; ++i)
    {
        if (character == DELIMITER)
        {
            // create card
            struct card_llist *card = (struct card_llist *)malloc(sizeof(struct card_llist));
            card->value = face_value_to_int(face_value);
            card->suit = suit;
            card->hidden = 1;
            card->next = NULL;

            add_card(&deck, card);

            i = -1;
            continue;
        }

        if (i == 0)
            face_value = character;
        else if (i == 1)
            suit = character;
        else if (i == 2)
            return NULL;
    }
    fclose(cards_file);

    return deck;
}
#else
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
#endif

int arrange_cards(struct card_llist *columns[COLUMNS])
{
    int columnSizes[COLUMNS] = {1, 6, 7, 8, 9, 10, 11};

    for (int i = 0; i < COLUMNS - 1; i++)
    {
        move_cards(columns[i], &columns[i + 1], columnSizes[i] - 1);
    }
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
            printf("\t%c%c\tF%d", '[', ']', foundationNr);
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

int print_columns(struct card_llist *column[COLUMNS])
{
    for (int i = 0; i < COLUMNS; i++)
    {
        printf("Column %d: ", i);
        struct card_llist *card = column[i];
        while (card != NULL)
        {
            printf("%c%c ", int_to_face_value(card->value), card->suit);
            card = card->next;
        }
        printf("\n");
    }
    return 0;
}

int deck_to_columns(struct card_llist *columns[COLUMNS], struct card_llist *deck)
{
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
    struct card_llist *card = get_card_by_index(column, index);
    if (card == NULL)
    {
        printf("No card found at index %d\n", index);
        return -1;
    }
    while (card != NULL)
    {
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
        // printf("inputArg: %s\n", inputArg);

        strcpy(message, "OK");
        if (strcmp(input, "ld") == 0 || strcmp(input, "LD") == 0) // Load deck
        {
            struct card_llist *deck;
#ifdef FILE_NAME
            deck = load_deck_from_file();
#else
            const char cards[CARD_COUNT][2] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC",
                                               "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD",
                                               "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH",
                                               "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS"};
            deck = load_cards_from_array(cards);
#endif
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
            for (int i = 0; i < COLUMNS; i++)
            {
                result = show_after_index(columns[i], 0);
                if (result != 0)
                {
                    strcpy(message, "Error showing cards");
                    continue;
                }
            }
            strcpy(lastCommand, "SW");
        }
        else if (strcmp(input, "si") == 0 || strcmp(input, "SI") == 0) // Shuffle, split and interleaves cards
        {
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

            result = shuffle_cards(&deck, splitIndex);
            if (result != 0)
                strcpy(message, "Error shuffling cards");

            for (int i = 0; i < COLUMNS; i++)
                columns[i] = NULL;
            result = deck_to_columns(columns, deck);
            if (result != 0)
            {
                strcpy(message, "Error moving cards to columns");
                continue;
            }
        }
        else if (strcmp(input, "sr") == 0 || strcmp(input, "SR") == 0) // Shuffle, insert randomly into new deck
        {
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
        }
        else if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0) // Quit current game
        {
        }
        else
        {
        }
    }
}
