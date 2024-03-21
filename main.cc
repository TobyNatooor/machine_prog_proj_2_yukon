#include <stdio.h>

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

void display_game(card_llist columns[], card_llist foundations[]);
void load_cards_from_file(card_llist columns[], card_llist foundations[]); // LD

int main(void)
{
    struct card_llist columns[COLUMNS]; // malloc(sizeof(card_llist) * COLUMNS);
    struct card_llist foundations[FOUNDATIONS];

#ifdef FILE_NAME
    load_cards_from_file(columns, foundations);
#else
    const char *cards[CARDS_NUM][2] = {"AC", "1C", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "JC", "QC", "KC",
                                       "AD", "1D", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "JD", "QD", "KD",
                                       "AH", "1H", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "JH", "QH", "KH",
                                       "AS", "1S", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "JS", "QS", "KS"};
#endif

    display_game(columns, foundations);

    return 0;
}

void load_cards_from_file(card_llist columns[], card_llist foundations[])
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
            // handle error
        }
        card[i] = character;
        i++;
    }
    // fclose(cards_file);
}

void display_game(card_llist columns[], card_llist foundations[])
{
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n");

    int row = 0;
    int columnEnd;
    bool breakOuter = false;
    for (int i = 0; columnEnd != COLUMNS; i++)
    {
        columnEnd = 0;
        for (int j = 0; j < COLUMNS; j++)
        {
            struct card_llist *nextCard = (columns + j);
            for (int k = 0; k < row; k++)
            {
                if (nextCard->next == NULL) {
                    columnEnd++;
                    printf("\t");
                    breakOuter = true;
                    break;
                }
                nextCard = nextCard->next;
            }
            if (breakOuter) {
                breakOuter = false;
                continue;
            }
            printf("%c%c\t", nextCard->card[0], nextCard->card[1]);
        }
        printf("\n");
        row++;
    }
}
