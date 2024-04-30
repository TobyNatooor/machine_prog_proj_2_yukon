#include "sdl_gui.h"

int render_cards(SDL_Renderer *renderer, struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS])
{
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

    for (int i = 0; i < COLUMNS; i++)
    {
        SDL_RenderDrawRect(renderer, &(SDL_Rect){30 + 120 * i, 30, CARD_WIDTH, CARD_HEIGHT});
        struct card_llist *card = columns[i];
        while (card != NULL)
        {
            char filename[100]; // Adjust the size as needed
            sprintf(filename, "../images/cards_pack/PNG/Medium/%i%c.png", card->value, card->suit);
            SDL_Surface *cardSurface = IMG_Load(filename);
            if (cardSurface == NULL)
                return -1;
            SDL_Texture *AC = SDL_CreateTextureFromSurface(renderer, cardSurface);
            SDL_RenderCopy(renderer, AC, NULL, &(SDL_Rect){30 + 120 * i, 30, CARD_WIDTH, CARD_HEIGHT});
            card = card->next;
        }
    }
    for (int i = 0; i < FOUNDATIONS; i++)
    {
        SDL_RenderDrawRect(renderer, &(SDL_Rect){960, 30 + 120 * i, CARD_WIDTH, CARD_HEIGHT});
        struct card_llist *card = columns[i];
        while (card != NULL)
        {
            char filename[100]; // Adjust the size as needed
            SDL_Surface *cardSurface = IMG_Load(filename);
            if (cardSurface == NULL)
                return -1;
            SDL_Texture *AC = SDL_CreateTextureFromSurface(renderer, cardSurface);
            SDL_RenderCopy(renderer, AC, NULL, &(SDL_Rect){960, 30 + 120 * i, CARD_WIDTH, CARD_HEIGHT});
            card = card->next;
        }
    }
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

    SDL_Window *window = SDL_CreateWindow("SDL_ttf Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font *font = TTF_OpenFont("../fonts/PlayFairDisplay-Regular.ttf", 24);
    SDL_Color color = {255, 255, 255}; // White

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, input, color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

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
                else if (e.key.keysym.sym >= SDLK_SPACE && e.key.keysym.sym <= SDLK_z)
                {
                    char newChar[2] = {e.key.keysym.sym, '\0'};
                    strcat(input, newChar);
                }
                else if (e.key.keysym.sym == SDLK_RETURN)
                {
                    char *response = handle_input(deck, columns, foundations, input, &inPlayPhase, &playing);
                    strcpy(input, response);
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
        int result = render_cards(renderer, columns, foundations);
        if (result != 0)
            return;

        if (textTexture)
        {
            SDL_Rect renderQuad = {0, WINDOW_HEIGHT - textSurface->h, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
        }
        SDL_RenderPresent(renderer);
    }

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
