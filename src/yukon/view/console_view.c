#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "console_view.h"

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

int print_cards(struct card_llist *cards)
{
    while (cards != NULL)
    {
        printf("%c%c ", int_to_face_value(cards->value), cards->suit);
        cards = cards->next;
    }
    printf("\n");
    return 0;
}

int print_columns(struct card_llist *column[COLUMNS])
{
    for (int i = 0; i < COLUMNS; i++)
    {
        printf("Column %d: ", i);
        print_cards(column[i]);
        printf("\n");
    }
    return 0;
}

int start_console_game()
{
    // initialize game
    struct card_llist *columns[COLUMNS];
    struct card_llist *foundations[FOUNDATIONS];
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    for (int i = 0; i < FOUNDATIONS; i++)
        foundations[i] = NULL;

    int playing = 1;
    int inPlayPhase = 0;
    char input[64];
    char message[64] = "";
    char lastCommand[64] = "";

    // game loop
    while (playing)
    {
        int result = display_game(columns, foundations);
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

        char *response = handle_input(columns, foundations, input, &inPlayPhase, &playing);
        strcpy(message, response);
        int won = won_game(columns);
        if (won)
        {
            printf("You won!\n");
            playing = 0;
        }
    }
}
