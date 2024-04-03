#include <stdio.h>
#include <string.h>

#define FILE_NAME "cards.txt"
#define CARDS_NUM 52
#define FOUNDATIONS 4
#define COLUMNS 7
#define CARD_SIZE 2

struct card_llist
{
    char card[2];
    bool hidden;
    struct card_llist *next;
};

int display_game(card_llist columns[], card_llist foundations[]);
int arrange_cards(card_llist columns[], card_llist foundations[]);
int load_cards_from_file(card_llist columns[], card_llist foundations[]);
// int load_cards_from_array(card_llist columns[], card_llist foundations[]);
card_llist *get_last_card(card_llist *column);

int main(void)
{
    // initialize game
    struct card_llist columns[COLUMNS];
    struct card_llist foundations[FOUNDATIONS];

#ifdef FILE_NAME
    load_cards_from_file(columns, foundations);
#else
    // const char *cards[CARDS_NUM][2] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC",
    //                                    "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD",
    //                                    "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH",
    //                                    "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS"};
    // load_cards_from_array(columns, foundations, cards);
#endif
    arrange_cards(columns, foundations);

    // game loop
    bool playing = true;
    while (playing)
    {
        display_game(columns, foundations);

        char input[128];
        printf("LAST Command: ");
        printf("\nMessage: ");
        printf("\nINPUT > ");
        scanf("%s", input);

        if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0)
        {
            playing = false;
        }
    }

    return 0;
}

card_llist *get_last_card(card_llist *column)
{
    struct card_llist *lastCard = column;
    while (lastCard->next != NULL)
    {
        lastCard = lastCard->next;
    }
    return lastCard;
}

int load_cards_from_file(card_llist columns[], card_llist foundations[])
{
    FILE *cards_file = fopen(FILE_NAME, "r");
    char character;
    char card[2];
    int i = 0;
    int card_count = 0;

    while ((character = fgetc(cards_file)) != EOF)
    {
        if (character == '\n')
        {
            int remainder = card_count % COLUMNS;
            int quotient = card_count / COLUMNS;
            struct card_llist *nextCard = (columns + remainder);
            for (int i = 0; i < quotient; i++)
            {
                if (nextCard->next == NULL)
                {
                    nextCard->next = new card_llist;
                }
                nextCard = nextCard->next;
            }
            nextCard->card[0] = card[0];
            nextCard->card[1] = card[1];
            nextCard->hidden = true;
            nextCard->next = NULL;

            // printf("%c%c\n", card[0], card[1]);
            card_count++;
            i = 0;
            continue;
        }
        if (i == 2)
        {
            return -1;
        }
        card[i] = character;
        i++;
    }

    fclose(cards_file);

    return 0;
}

int arrange_cards(card_llist columns[], card_llist foundations[])
{
    // 1, 6, 7, 8, 9, 10, and 11
    int columnSizes[COLUMNS] = {1, 6, 7, 8, 9, 10, 11};

    for (int i = 0; i < COLUMNS; i++)
    {
        struct card_llist *lastColumnCard = (columns + i);
        for (int j = 0; j < columnSizes[i] - 1; j++)
        {
            if (lastColumnCard->next == NULL)
            {
                return -1;
            }
            lastColumnCard = lastColumnCard->next;
        }
        if (COLUMNS - 1 == i)
        {
            break;
        }
        get_last_card(columns + i + 1)->next = lastColumnCard->next;
        lastColumnCard->next = NULL;
    }
    return 0;
}

int display_game(card_llist columns[], card_llist foundations[])
{
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    int row = 0;
    int columnEnd = 0;
    bool breakOuter = false;
    for (int i = 0; columnEnd != COLUMNS; i++)
    {
        // columns
        columnEnd = 0;
        for (int j = 0; j < COLUMNS; j++)
        {
            struct card_llist *nextCard = (columns + j);
            for (int k = 0; k < row; k++)
            {
                if (nextCard->next == NULL)
                {
                    columnEnd++;
                    printf("\t");
                    breakOuter = true;
                    break;
                }
                nextCard = nextCard->next;
            }
            if (breakOuter)
            {
                breakOuter = false;
                continue;
            }
            printf("%c%c\t", nextCard->card[0], nextCard->card[1]);
        }

        // foundations
        if (row == 0 || row == 2 || row == 4 || row == 6)
        {
            int foundationNr = row / 2 + 1;
            printf("\t%c%c\tF%d", '[', ']', foundationNr);
        }

        printf("\n");
        row++;
    }
    return 0;
}
