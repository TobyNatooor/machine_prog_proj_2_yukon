#include <SDL.h>
#include <SDL_ttf.h>

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("SDL_ttf Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font *font = TTF_OpenFont("../fonts/PlayFairDisplay-Regular.ttf", 24);
    SDL_Color color = {255, 255, 255}; // White

    char text[100] = "Type something!";
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int running = 0;

    SDL_Event e;
    while (!running)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                running = 1;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(text) > 0)
                {
                    text[strlen(text) - 1] = '\0';
                }
                else if (e.key.keysym.sym >= SDLK_SPACE && e.key.keysym.sym <= SDLK_z)
                {
                    char newChar[2] = {e.key.keysym.sym, '\0'};
                    strcat(text, newChar);
                }

                // Re-render text
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
                textSurface = TTF_RenderText_Solid(font, text, color);
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            }
        }

        SDL_RenderClear(renderer);
        if (textTexture)
        {
            SDL_Rect renderQuad = {0, 0, textSurface->w, textSurface->h};
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

    return 0;
}
