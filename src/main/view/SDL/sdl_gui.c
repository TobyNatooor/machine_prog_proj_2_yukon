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
                SDL_RenderCopy(renderer, cardTextures[CARD_COUNT], NULL, &(SDL_Rect){30 + 120 * i, 30 + j * 20, CARD_WIDTH, CARD_HEIGHT});
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
                SDL_RenderCopy(renderer, cardTextures[card->value + 13 * cardSuitInt - 1], NULL, &(SDL_Rect){30 + 120 * i, 30 + j * 20, CARD_WIDTH, CARD_HEIGHT});
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

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("Yukon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

    TTF_Font *font = TTF_OpenFont("../fonts/PlayFairDisplay-Regular.ttf", 24);
    SDL_Color color = {255, 255, 255}; // White

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, input, color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Texture *cardTextures[CARD_COUNT + 1];
    if (load_card_textures(renderer, cardTextures) != 0)
        return;

    SDL_StartTextInput();

    int running = 1;

    SDL_Event e;
    while (running)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (e.type == SDL_KEYDOWN)
            {
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
                    strcpy(input, "");
                    SDL_Log("Response: %s", response);
                }

                // Re-render text
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
                textSurface = TTF_RenderText_Solid(font, input, color);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 80, 0, 255);
        SDL_RenderClear(renderer);
        int result = render_cards(renderer, columns, foundations, cardTextures);
        if (result != 0)
            return;

        if (textTexture)
        {
            SDL_Rect renderQuad = {0, WINDOW_HEIGHT - textSurface->h, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
        }
        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    for (int i = 0; i < CARD_COUNT + 1; i++)
        SDL_DestroyTexture(cardTextures[i]);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
