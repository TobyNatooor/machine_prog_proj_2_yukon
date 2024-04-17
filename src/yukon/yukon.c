#include "yukon.h"

struct card_llist *load_cards_from_file(char fileName[])
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

int get_input(char input[], char inputArg[])
{
    char *token = strtok(input, " ");
    if (token == NULL)
        return -1;
    strcpy(input, token);
    token = strtok(NULL, " ");
    if (token != NULL)
    {
        if (strtok(NULL, " ") != NULL)
            return -1;
        strcpy(inputArg, token);
    }
    else
    {
        strcpy(inputArg, "");
    }
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
