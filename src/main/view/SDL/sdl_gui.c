#include <stdbool.h>
#include "sdl_gui.h"

static const enum suits suit_list[] = {HEARTS, DIAMONDS, CLUBS, SPADES};

int load_card_textures(SDL_Renderer *renderer, SDL_Texture *cardTextures[CARD_COUNT + 1])
{
    for (int i = 0; i < CARD_COUNT; i++)
    {
        char filename[64];
        sprintf(filename, "../images/cards_pack/PNG/Medium/%i%c.png", i % 13 + 1, suit_list[i / 13]);
        SDL_Surface *cardSurface = IMG_Load(filename);
        if (cardSurface == NULL)
            return -1;
        cardTextures[i] = SDL_CreateTextureFromSurface(renderer, cardSurface);
    }
    SDL_Surface *backSide = IMG_Load("../images/cards_pack/PNG/Medium/Back Blue 2.png");
    if (backSide == NULL)
        return -1;
    cardTextures[CARD_COUNT] = SDL_CreateTextureFromSurface(renderer, backSide);

    return 0;
}

int render_cards(SDL_Renderer *renderer, struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], SDL_Texture *cardTextures[CARD_COUNT + 1])
{
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

    for (int i = 0; i < COLUMNS; i++)
    {
        SDL_RenderDrawRect(renderer, &(SDL_Rect){30 + 120 * i, 30, CARD_WIDTH, CARD_HEIGHT});
        struct card_llist *card = columns[i];
        for (int j = 0; card != NULL; j++)
        {
            if (card->hidden)
                SDL_RenderCopy(renderer, cardTextures[CARD_COUNT], NULL, &(SDL_Rect){30 + 120 * i, 30 + j * 40, CARD_WIDTH, CARD_HEIGHT});
            else
            {
                int cardSuitInt;
                switch (card->suit)
                {
                case HEARTS:
                    cardSuitInt = 0;
                    break;
                case DIAMONDS:
                    cardSuitInt = 1;
                    break;
                case CLUBS:
                    cardSuitInt = 2;
                    break;
                case SPADES:
                    cardSuitInt = 3;
                    break;
                }
                SDL_RenderCopy(renderer, cardTextures[card->value + 13 * cardSuitInt - 1], NULL, &(SDL_Rect){30 + 120 * i, 30 + j * 40, CARD_WIDTH, CARD_HEIGHT});
            }

            card = card->next;
        }
    }
    for (int i = 0; i < FOUNDATIONS; i++)
    {
        SDL_RenderDrawRect(renderer, &(SDL_Rect){960, 30 + 120 * i, CARD_WIDTH, CARD_HEIGHT});
        struct card_llist *card = get_last_card(foundations[i]);
        if (card != NULL)
        {
            int cardSuitInt;
            switch (card->suit)
            {
            case HEARTS:
                cardSuitInt = 0;
                break;
            case DIAMONDS:
                cardSuitInt = 1;
                break;
            case CLUBS:
                cardSuitInt = 2;
                break;
            case SPADES:
                cardSuitInt = 3;
                break;
            }
            SDL_RenderCopy(renderer, cardTextures[card->value + 13 * cardSuitInt - 1], NULL, &(SDL_Rect){960, 30 + 120 * i, CARD_WIDTH, CARD_HEIGHT});
        }
    }
    return 0;
}

//ADD Quit pop up.
bool confirm_quit(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color)
{
    SDL_Surface *messageSurface = TTF_RenderText_Solid(font, "Do you really want to quit?", color);
    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);

    SDL_Surface *yesSurface = TTF_RenderText_Solid(font, "Yes", color);
    SDL_Texture *yesTexture = SDL_CreateTextureFromSurface(renderer, yesSurface);

    SDL_Surface *noSurface = TTF_RenderText_Solid(font, "No", color);
    SDL_Texture *noTexture = SDL_CreateTextureFromSurface(renderer, noSurface);

    int messageWidth, messageHeight;
    SDL_QueryTexture(messageTexture, NULL, NULL, &messageWidth, &messageHeight);

    int yesWidth, yesHeight;
    SDL_QueryTexture(yesTexture, NULL, NULL, &yesWidth, &yesHeight);

    int noWidth, noHeight;
    SDL_QueryTexture(noTexture, NULL, NULL, &noWidth, &noHeight);

    SDL_Rect messageRect = {WINDOW_WIDTH / 2 - messageWidth / 2, WINDOW_HEIGHT / 2 - messageHeight - 20, messageWidth, messageHeight};
    SDL_Rect yesRect = {WINDOW_WIDTH / 2 - yesWidth - 20, WINDOW_HEIGHT / 2 + 20, yesWidth, yesHeight};
    SDL_Rect noRect = {WINDOW_WIDTH / 2 + 20, WINDOW_HEIGHT / 2 + 20, noWidth, noHeight};

    bool running = true;
    bool quitConfirmed = false;
    SDL_Event e;

    while (running)
    {
        // Handle events
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
                quitConfirmed = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                    quitConfirmed = false;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= yesRect.x && x <= yesRect.x + yesRect.w && y >= yesRect.y && y <= yesRect.y + yesRect.h)
                {
                    running = false;
                    quitConfirmed = true;
                }
                else if (x >= noRect.x && x <= noRect.x + noRect.w && y >= noRect.y && y <= noRect.y + noRect.h)
                {
                    running = false;
                    quitConfirmed = false;
                }
            }
        }

        // Render the popup
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200); // Semi-transparent background
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
        SDL_RenderCopy(renderer, yesTexture, NULL, &yesRect);
        SDL_RenderCopy(renderer, noTexture, NULL, &noRect);

        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
    SDL_FreeSurface(yesSurface);
    SDL_DestroyTexture(yesTexture);
    SDL_FreeSurface(noSurface);
    SDL_DestroyTexture(noTexture);

    return quitConfirmed;
}

void start_sdl_game()
{
    struct card_llist *deck[CARD_COUNT];
    struct card_llist *columns[COLUMNS];
    struct card_llist *foundations[FOUNDATIONS];
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    for (int i = 0; i < FOUNDATIONS; i++)
        foundations[i] = NULL;

    int playing = 1;
    int inPlayPhase = 0;
    char *input = malloc(64 * sizeof(char));
    input[0] = '\0';
    char *message = malloc(64 * sizeof(char));
    message[0] = '\0';
    char *lastCommand = malloc(64 * sizeof(char));
    lastCommand[0] = '\0';

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("Yukon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

    //TTF_Font *font = TTF_OpenFont("../fonts/PlayfairDisplay-Regular.ttf", 24);
    TTF_Font *font = TTF_OpenFont("../fonts/DiaryOfAn8BitMage-lYDD.ttf", 24);
    TTF_Font *smallFont = TTF_OpenFont("../fonts/DiaryOfAn8BitMage-lYDD.ttf", 18);  // Smaller size for active console
    SDL_Color color = {255, 255, 255}; // Font color: White

    //Render the welcome message
    SDL_Surface *welcomeSurface = TTF_RenderText_Solid(font, "Welcome to Yukon game! Click anywhere on this screen to start the game.", color);
    SDL_Texture *welcomeTexture = SDL_CreateTextureFromSurface(renderer, welcomeSurface);
    SDL_SetRenderDrawColor(renderer, 255, 205, 210, 100);  // Welcome Background color

    // Load the image
    SDL_Surface *imageSurface = IMG_Load("../images/yukon_icon200.png");
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);

    // Render "Input:" message immediately with small font
    SDL_Surface *commandSurface = TTF_RenderText_Solid(smallFont, "Input:", color);
    SDL_Texture *commandTexture = SDL_CreateTextureFromSurface(renderer, commandSurface);

    // Get the width and height of the "Input:" text
    int commandWidth = 0;
    int commandHeight = 0;
    SDL_QueryTexture(commandTexture, NULL, NULL, &commandWidth, &commandHeight);

    // Define a padding value
    int padding = 10;

    // Calculate the positions for the LAST Command, Message, and Input texts
    int baseY = WINDOW_HEIGHT - (commandHeight + 20); // Bottom padding of 20
    int messageY = baseY - commandHeight * 2- padding;         // Message above Input
    int lastCommandY = messageY - commandHeight- padding;      // LAST Command above Message


    SDL_RenderClear(renderer);

    // Render the image
    if (imageTexture && imageSurface)
    {
        SDL_Rect imageQuad = {WINDOW_WIDTH / 2 - imageSurface->w / 2, WINDOW_HEIGHT / 2 - imageSurface->h - 50, imageSurface->w, imageSurface->h};
        SDL_RenderCopy(renderer, imageTexture, NULL, &imageQuad);
    }

    // Render the welcome message
    if (welcomeTexture && welcomeSurface)
    {
        SDL_Rect renderQuad = {50, WINDOW_HEIGHT / 2 - welcomeSurface->h, welcomeSurface->w, welcomeSurface->h};
        SDL_RenderCopy(renderer, welcomeTexture, NULL, &renderQuad);
        SDL_RenderPresent(renderer);
    }

    // Wait for a key press to continue
    int continueGame = 0;
    SDL_Event e;
    while (!continueGame)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                continueGame = 1;
                playing = 0;  // Exit the game
            }
            else if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
            {
                continueGame = 1;  // Proceed to the game loop
            }
        }
    }

    // Clean up welcome message resources
    SDL_FreeSurface(welcomeSurface);
    SDL_DestroyTexture(welcomeTexture);
    SDL_FreeSurface(imageSurface);
    SDL_DestroyTexture(imageTexture);

    SDL_Texture *cardTextures[CARD_COUNT + 1];
    if (load_card_textures(renderer, cardTextures) != 0)
        return;

    SDL_SetRenderDrawColor(renderer, 0, 96, 100, 100);
    SDL_RenderClear(renderer);

    int result = render_cards(renderer, columns, foundations, cardTextures);
    if (result != 0)
        return;

    // Render the "Insert command:" message on the left bottom corner
    if (commandTexture && commandSurface)
    {
        SDL_Rect commandRenderQuad = {20, WINDOW_HEIGHT - commandSurface->h - 20, commandSurface->w, commandSurface->h};
        SDL_RenderCopy(renderer, commandTexture, NULL, &commandRenderQuad);
    }

    // Set up the initial input position to be right after the "Input:" text
    SDL_Rect inputRenderQuad = {20 + commandWidth + 10, baseY, 0, 0};

    SDL_Surface *inputSurface = TTF_RenderText_Solid(smallFont, input, color);
    SDL_Texture *inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);

    SDL_Surface *messageSurface = TTF_RenderText_Solid(smallFont, message, color);
    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    SDL_Surface *lastCommandSurface = TTF_RenderText_Solid(smallFont, lastCommand, color);
    SDL_Texture *lastCommandTexture = SDL_CreateTextureFromSurface(renderer, lastCommandSurface);

    SDL_RenderPresent(renderer);

    SDL_StartTextInput();

    int running = 1;

    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                if (confirm_quit(renderer,smallFont,color))
                {
                    running = 0;
                }
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    if (confirm_quit(renderer, smallFont, color))
                    {
                        running = 0;
                    }
                }

                if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(input) > 0)
                {
                    input[strlen(input) - 1] = '\0';
                }
                else if (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z || 48 <= e.key.keysym.sym && e.key.keysym.sym <= 57 || e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == 45 || e.key.keysym.sym == 46 || e.key.keysym.sym == 60)
                {
                    if (SDL_GetModState() & KMOD_SHIFT)
                    {
                        if (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z)
                        {
                            char newChar[2] = {e.key.keysym.sym - 32, '\0'};
                            strcat(input, newChar);
                        }
                        else if (e.key.keysym.sym == 45)
                        {
                            char newChar[2] = {'_', '\0'};
                            strcat(input, newChar);
                        }
                        else if (e.key.keysym.sym == 46)
                        {
                            char newChar[2] = {':', '\0'};
                            strcat(input, newChar);
                        }
                        else if (e.key.keysym.sym == 60)
                        {
                            char newChar[2] = {'>', '\0'};
                            strcat(input, newChar);
                        }
                    }
                    else
                    {
                        char newChar[2] = {e.key.keysym.sym, '\0'};
                        strcat(input, newChar);
                    }
                }
                else if (e.key.keysym.sym == SDLK_RETURN)
                {
                    char *response = handle_input(deck, columns, foundations, input, &inPlayPhase, &playing);
                    strcpy(message, "Message: ");
                    strcat(message, response);
                    strcpy(lastCommand, "LAST Command: ");
                    strcat(lastCommand, input);
                    strcpy(input, "");

                    SDL_Log("Response: %s", response);
                }

                SDL_FreeSurface(inputSurface);
                SDL_DestroyTexture(inputTexture);

                SDL_FreeSurface(messageSurface);
                SDL_DestroyTexture(messageTexture);
                SDL_FreeSurface(lastCommandSurface);
                SDL_DestroyTexture(lastCommandTexture);

                inputSurface = TTF_RenderText_Solid(font, input, color);
                inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);

                // Update the width of the input text for rendering position adjustment
                int inputWidth;
                SDL_QueryTexture(inputTexture, NULL, NULL, &inputWidth, NULL);
                inputRenderQuad.w = inputWidth;
                inputRenderQuad.h = commandHeight;

                messageSurface = TTF_RenderText_Solid(font, message, color);
                messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
                lastCommandSurface = TTF_RenderText_Solid(font, lastCommand, color);
                lastCommandTexture = SDL_CreateTextureFromSurface(renderer, lastCommandSurface);
            }
        }

        //Original color
        //SDL_SetRenderDrawColor(renderer, 0, 80, 0, 255);
        SDL_SetRenderDrawColor(renderer, 0, 96, 100, 100);// the board background color
        SDL_RenderClear(renderer);

        //Start to render the card row
        // Render the column labels C1 to C7
        for (int i = 0; i < COLUMNS; i++) {
            char label[3];
            snprintf(label, sizeof(label), "C%d", i + 1);
            SDL_Surface *labelSurface = TTF_RenderText_Solid(smallFont, label, color);
            SDL_Texture *labelTexture = SDL_CreateTextureFromSurface(renderer, labelSurface);

            int labelWidth, labelHeight;
            SDL_QueryTexture(labelTexture, NULL, NULL, &labelWidth, &labelHeight);

            SDL_Rect labelRect = {30 + 120 * i + CARD_WIDTH / 2 - labelWidth / 2, 10, labelWidth, labelHeight};
            SDL_RenderCopy(renderer, labelTexture, NULL, &labelRect);

            SDL_FreeSurface(labelSurface);
            SDL_DestroyTexture(labelTexture);
        }

        // Render the foundation labels F1 to F4
        for (int i = 0; i < FOUNDATIONS; i++) {
            char label[3];
            snprintf(label, sizeof(label), "F%d", i + 1);
            SDL_Surface *labelSurface = TTF_RenderText_Solid(smallFont, label, color);
            SDL_Texture *labelTexture = SDL_CreateTextureFromSurface(renderer, labelSurface);

            int labelWidth, labelHeight;
            SDL_QueryTexture(labelTexture, NULL, NULL, &labelWidth, &labelHeight);

            SDL_Rect labelRect = {960 + CARD_WIDTH / 2 - labelWidth / 2, 30 + 120 * i + CARD_HEIGHT / 2 - labelHeight / 2, labelWidth, labelHeight};
            SDL_RenderCopy(renderer, labelTexture, NULL, &labelRect);

            SDL_FreeSurface(labelSurface);
            SDL_DestroyTexture(labelTexture);
        }


        int result = render_cards(renderer, columns, foundations, cardTextures);
        if (result != 0)
            return;

        // Render the LAST Command text
        if (lastCommandTexture && lastCommandSurface)
        {
            SDL_Rect lastCommandRenderQuad = {20, lastCommandY, lastCommandSurface->w, lastCommandSurface->h};
            SDL_RenderCopy(renderer, lastCommandTexture, NULL, &lastCommandRenderQuad);
        }

        // Render the Message text
        if (messageTexture && messageSurface)
        {
            SDL_Rect messageRenderQuad = {20, messageY, messageSurface->w, messageSurface->h};
            SDL_RenderCopy(renderer, messageTexture, NULL, &messageRenderQuad);
        }

        // Render the "Input:" prompt
        if (commandTexture && commandSurface)
        {
            SDL_Rect commandRenderQuad = {20, baseY, commandSurface->w, commandSurface->h};
            SDL_RenderCopy(renderer, commandTexture, NULL, &commandRenderQuad);
        }

        // Render the input text right after "Input:"
        if (inputTexture && inputSurface)
        {
            SDL_RenderCopy(renderer, inputTexture, NULL, &inputRenderQuad);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();

    // Clean up resources and quit
    SDL_FreeSurface(inputSurface);
    SDL_DestroyTexture(inputTexture);
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
    SDL_FreeSurface(lastCommandSurface);
    SDL_DestroyTexture(lastCommandTexture);
    for (int i = 0; i < CARD_COUNT + 1; i++)
        SDL_DestroyTexture(cardTextures[i]);
    TTF_CloseFont(font);
    TTF_CloseFont(smallFont);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    IMG_Quit();  // Quit SDL_image
}
