#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "yukon.h"

#define FILE_NAME "../../cards.txt"
#define CARDS_NUM 52
#define FOUNDATIONS 4
#define COLUMNS 7
#define CARD_SIZE 2

struct card_llist *get_last_card(struct card_llist *column)
{
    if (column == NULL)
        return NULL;
    struct card_llist *lastCard = column;
    while (lastCard->next != NULL)
    {
        lastCard = lastCard->next;
    }
    return lastCard;
}

void add_card(struct card_llist *column, struct card_llist *card)
{
    if (column == NULL)
    {
        column = card;
        return;
    }
    while (column->next != NULL)
    {
        column = column->next;
    }
    column->next = card;
}

struct card_llist *get_card_by_index(struct card_llist *column, int index)
{
    struct card_llist *card = column;
    for (int i = 0; i < index; i++)
    {
        if (card == NULL)
            return NULL;
        card = card->next;
    }
    return card;
}

void move_cards(struct card_llist *from, struct card_llist *to, int index)
{
    struct card_llist *lastCard = get_last_card(from);
    struct card_llist *newLastCard = get_card_by_index(from, index - 1);
    lastCard->next = to;
    from = newLastCard;
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

int load_cards_from_file(struct card_llist *columns[])
{
    FILE *cards_file = fopen(FILE_NAME, "r");
    char character;
    char face_value;
    char suit;

    for (int i = 0; (character = fgetc(cards_file)) != EOF; i++)
    {
        if (character == '\n')
        {
            struct card_llist *card = (struct card_llist *)malloc(sizeof(struct card_llist));

            card->value = face_value_to_int(face_value);
            card->suit = suit;
            card->hidden = 1;
            card->next = NULL;
            add_card(columns[0], card);
            i = 0;
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

int arrange_cards(struct card_llist *columns[], struct card_llist *foundations[])
{
    // 1, 6, 7, 8, 9, 10, and 11
    int columnSizes[COLUMNS] = {1, 6, 7, 8, 9, 10, 11};

    for (int i = 0; i < COLUMNS; i++)
    {
        move_cards(columns[0], columns[i], columnSizes[i]);
    }
    return 0;
}

int display_game(struct card_llist *columns[], struct card_llist *foundations[])
{
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    int rows = 0;
    int columnsDone = 0;
    for (int rows = 0; columnsDone != COLUMNS || rows < 7; rows++)
    {
        // columns
        columnsDone = 0;
        for (int j = 0; j < COLUMNS; j++)
        {
            struct card_llist *nextCard = get_card_by_index(columns[j], j);
            if (nextCard == NULL)
            {
                printf("\t");
            }
            else
            {
                printf("%c%c\t", int_to_face_value(nextCard->value), nextCard->suit);
            }
        }

        // foundations
        if (rows == 0 || rows == 2 || rows == 4 || rows == 6)
        {
            int foundationNr = rows / 2 + 1;
            printf("\t%c%c\tF%d", '[', ']', foundationNr);
        }
        printf("\n");
    }
    return 0;
}

void print_column(struct card_llist *column)
{
    if (column == NULL)
    {
        printf("Empty column\n");
        return;
    }
    while (column != NULL)
    {
        printf("%c%c ", int_to_face_value(column->value), column->suit);
        column = column->next;
    }
    printf("\n");
}

int start_game()
{
    // initialize game
    struct card_llist *columns[COLUMNS];
    struct card_llist *foundations[FOUNDATIONS];
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    if (columns == NULL)
    {
        printf("NULL\n");
    }
    for (int i = 0; i < FOUNDATIONS; i++)
        foundations[i] = NULL;

#ifdef FILE_NAME
    int result = load_cards_from_file(columns);
    if (result != 0)
    {
        printf("Error loading cards from file\n");
        return -1;
    }
#else
        // const char *cards[CARDS_NUM][2] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC",
        //                                    "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD",
        //                                    "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH",
        //                                    "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS"};
        // load_cards_from_array(columns, foundations, cards);
#endif
    print_column(columns[0]);
    arrange_cards(columns, foundations);

    // game loop
    int playing = 1;
    while (playing)
    {
        display_game(columns, foundations);

        char input[128];
        printf("LAST Command: ");
        printf("\nMessage: ");
        printf("\nINPUT > ");
        scanf("%s", input);

        if (strcmp(input, "qq") == 0 || strcmp(input, "QQ") == 0)
        {
            // remove cards
            playing = 0;
        }
    }
}

int main(void)
{
    start_game();
    return 0;
}
