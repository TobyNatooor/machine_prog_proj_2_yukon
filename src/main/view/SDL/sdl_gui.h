#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "sdl_defines.h"
#include "../../yukon_commands.h"

void start_sdl_game();
int render_cards(SDL_Renderer *renderer, struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS]);
