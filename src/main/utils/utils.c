#include "utils.h"

int load_cards_from_file(struct card_llist *deck[CARD_COUNT], char fileName[])
{
    char tempCards[CARD_COUNT][CARD_SIZE];
    FILE *cards_file = fopen(fileName, "r");
    // if 'cards.txt' isn't found, try in the parent directory
    if (cards_file == NULL)
    {
        char path[32] = "../";
        strcat(path, fileName);
        cards_file = fopen(path, "r");
    }
    if (cards_file == NULL)
        return -1;

    int i = 0;
    char character, faceValue, suit;
    for (int j = 0; 1; ++j)
    {
        character = fgetc(cards_file);
        if (character == DELIMITER || character == EOF)
        {
            //printf("Loop nr.: %d - storing card %c%c\n", i, faceValue, suit);
            if(i > 0 && checkDuplicateInArray(tempCards, (i-1), faceValue, suit))
                return -1;
            tempCards[i][0] = faceValue;
            tempCards[i][1] = suit;
            //printf("Loop nr.: %d - stored card %c%c\n", i, tempCards[i][0], tempCards[i][1]);
            i++;
            j = -1;
            if(character == EOF)
                break;
            continue;

        }

        if (j == 0)
            faceValue = character;
        else if (j == 1)
            suit = character;
        else if (j == 2)
            return -1;

    }
    if(tempCards[CARD_COUNT-1][0] == NULL) {
        printf("Does this activate?\n");
        return -1;
    }
    fclose(cards_file);
    return load_cards_from_array(deck, tempCards);
}

int checkDuplicateInArray(char cards[CARD_COUNT][CARD_SIZE], int index, char faceValue, char suit)
{
    for (int i = 0; i < index; i++)
    {
        if (cards[i][0] == faceValue && cards[i][1] == suit)
            return 1;
    }
    return 0;
}

int checkDuplicate(struct card_llist *deck[CARD_COUNT], int index, char faceValue, char suit)
{
    for (int i = 0; i < index; i++)
    {
        if (deck[i]->value == face_value_to_int(faceValue) && deck[i]->suit == suit)
            return 1;
    }
    return 0;
}

int load_cards_from_array(struct card_llist *deck[CARD_COUNT], const char cards[CARD_COUNT][CARD_SIZE])
{
    for (int i = 0; i < CARD_COUNT; i++)
    {
        // create card
        struct card_llist *card = (struct card_llist *)malloc(sizeof(struct card_llist));
        card->value = face_value_to_int(cards[i][0]);
        card->suit = cards[i][1];
        card->hidden = 1;
        card->next = NULL;

        deck[i] = card;
        if (deck[i] == NULL)
            return -1;
    }
    return 0;
}

int arrange_cards(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT])
{
    const int columnSizes[COLUMNS] = {1, 6, 7, 8, 9, 10, 11};
    const int columnsIndexShown[COLUMNS] = {0, 1, 2, 3, 4, 5, 6};

    for (int i = 0; i < CARD_COUNT; i++)
        deck[i]->next = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    int j = 0;
    for (int i = 0; i < CARD_COUNT; i++)
    {
        while (get_cards_size(columns[j % COLUMNS]) == columnSizes[j % COLUMNS])
            j++;
        struct card_llist *card = deck[i];
        add_card(&columns[j % COLUMNS], card);
        j++;
    }
    for (int i = 0; i < COLUMNS; i++)
        show_after_index(columns[i], columnsIndexShown[i]);

    return 0;
}

int deck_to_columns(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT])
{
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;

    for (int i = 0; i < CARD_COUNT; i++)
    {
        deck[i]->next = NULL;
        int result = add_card(&columns[i % COLUMNS], deck[i]);
        if (result != 0)
            return -1;
    }
    return 0;
}

struct card_llist *deck_to_card_llist(struct card_llist *deck[CARD_COUNT])
{
    struct card_llist *card_llist = NULL;
    for (int i = 0; i < CARD_COUNT; i++)
    {
        if (deck[i] == NULL)
            return NULL;
        if (deck[i]->next != NULL)
            deck[i]->next = NULL;
        int result = add_card(&card_llist, deck[i]);
        if (result != 0)
            return NULL;
    }
    return card_llist;
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

int won_game(struct card_llist *columns[COLUMNS])
{
    int allColumnsEmpty = 1;
    for (int i = 0; i < COLUMNS; i++)
    {
        if (columns[i] != NULL)
        {
            return 0;
        }
    }
    return 1;
}

char *get_command(char *input)
{
    char *copy = strdup(input);
    char *command = strtok(copy, " ");
    if (command == NULL)
        return NULL;
    return command;
}

char *get_argument(char *input)
{
    char *copy = strdup(input);
    char *command = strtok(copy, " ");
    if (command == NULL)
        return NULL;
    char *argument = strtok(NULL, " ");
    if (argument == NULL || strcmp(argument, "") == 0)
        return NULL;
    else
    {
        return argument;
    }
}
