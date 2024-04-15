#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "yukon.h"

#define DELIMITER '\n'

#ifdef FILE_NAME
int load_cards_from_file(struct card_llist *columns[COLUMNS])
{
    FILE *cards_file = fopen(FILE_NAME, "r");
    // if 'cards.txt' isn't found, try in the parent directory
    if (cards_file == NULL) {
        char path[32] = "../";
        strcat(path, FILE_NAME);
        cards_file = fopen(path, "r");
    }
    if (cards_file == NULL)
    {
        printf("Error opening file\n");
        return -1;
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

            add_card(&columns[0], card);

            i = -1;
            continue;
        }

        if (i == 0)
            face_value = character;
        else if (i == 1)
            suit = character;
        else if (i == 2)
            return -1;
    }
    fclose(cards_file);

    return 0;
}
#else
int load_cards_from_array(struct card_llist *columns[COLUMNS], const char cards[CARD_COUNT][CARD_SIZE])
{
    for (int i = 0; i < CARD_COUNT; i++)
    {
        // create card
        struct card_llist *card = (struct card_llist *)malloc(sizeof(struct card_llist));
        card->value = face_value_to_int(cards[i][0]);
        card->suit = cards[i][1];
        card->hidden = 1;
        card->next = NULL;

        add_card(&columns[0], card);
    }
    return 0;
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

int start_game()
{
    // initialize game
    struct card_llist *columns[COLUMNS];
    struct card_llist *foundations[FOUNDATIONS];
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    for (int i = 0; i < FOUNDATIONS; i++)
        foundations[i] = NULL;

    int result;
#ifdef FILE_NAME
    result = load_cards_from_file(columns);
#else
    const char cards[CARD_COUNT][2] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC",
                                      "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD",
                                      "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH",
                                      "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS"};
    result = load_cards_from_array(columns, cards);
#endif
    if (result != 0)
    {
        printf("Error loading cards\n");
        return -1;
    }
    result = shuffle_cards(&columns[0]);
    if (result != 0)
    {
        printf("Error shuffling cards\n");
        return -1;
    }

    result = arrange_cards(columns);
    if (result != 0)
    {
        printf("Error arranging cards\n");
        return -1;
    }

    // game loop
    int playing = 1;
    while (playing)
    {
        result = display_game(columns, foundations);
        if (result != 0)
        {
            printf("Error displaying game\n");
            return -1;
        }

        char input[32];
        printf("LAST Command: ");
        printf("\nMessage: ");
        printf("\nINPUT > ");
        scanf("%s", input);

        if (strcmp(input, "qq") == 0 || strcmp(input, "QQ") == 0)
        {
            for (int i = 0; i < COLUMNS; i++)
                remove_cards(columns[i]);
            playing = 0;
        }
    }
}
