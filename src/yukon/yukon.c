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

int show_after_index(struct card_llist *column[COLUMNS], int index) {
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

    // result = shuffle_cards(&columns[0]);
    // if (result != 0)
    // {
    //     printf("Error shuffling cards\n");
    //     return -1;
    // }

    // result = arrange_cards(columns);
    // if (result != 0)
    // {
    //     printf("Error arranging cards\n");
    //     return -1;
    // }

    // game loop
    int result;
    int playing = 1;
    char input[64];
    char message[64];
    char lastCommand[2];
    while (playing)
    {
        result = display_game(columns, foundations);
        if (result != 0)
            strcpy(message, "Error displaying game");

        printf("LAST Command: %c%c", lastCommand[0], lastCommand[1]);
        printf("\nMessage: ");
        printf("%s", message);
        printf("\nINPUT > ");
        scanf("%s", input);

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
                strcpy(message, "Error loading cards");

            result = deck_to_columns(columns, deck);
            if (result != 0)
                strcpy(message, "Error moving cards to columns");

            lastCommand[0] = 'L';
            lastCommand[1] = 'D';
        }
        else if (strcmp(input, "sw") == 0 || strcmp(input, "SW") == 0) // Show
        {
            for (int i = 0; i < COLUMNS; i++)
            {
                result = show_after_index(columns[i], 0);
                if (result != 0)
                    strcpy(message, "Error showing cards");
            }
            lastCommand[0] = 'S';
            lastCommand[1] = 'W';
        }
        else if (strcmp(input, "si") == 0 || strcmp(input, "SI") == 0) // Shuffle, split and interleaves cards
        {
            lastCommand[0] = 'S';
            lastCommand[1] = 'I';
        }
        else if (strcmp(input, "sr") == 0 || strcmp(input, "SR") == 0) // Shuffle, insert randomly into new deck
        {
        }
        else if (strcmp(input, "sd") == 0 || strcmp(input, "SD") == 0) // Save deck
        {
            lastCommand[0] = 'S';
            lastCommand[1] = 'D';
        }
        else if (strcmp(input, "qq") == 0 || strcmp(input, "QQ") == 0) // Quit program
        {
            for (int i = 0; i < COLUMNS; i++)
                remove_cards(columns[i]);
            playing = 0;
        }
        else if (strcmp(input, "p") == 0 || strcmp(input, "P") == 0) // Play
        {
            lastCommand[0] = 'P';
            lastCommand[1] = ' ';
        }
        else if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0) // Quit current game
        {
            lastCommand[0] = 'Q';
            lastCommand[1] = ' ';
        }
        else
        {
        }
    }
}
