#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "sdl_defines.h"
#include "../../yukon_commands.h"

int load_card_textures(SDL_Renderer *renderer, SDL_Texture *cardTextures[CARD_COUNT + 1]);
int render_cards(SDL_Renderer *renderer, struct card_llist *columns[COLUMNS], struct card_llist *foundations[FOUNDATIONS], SDL_Texture *cardTextures[CARD_COUNT + 1]);
void start_sdl_game();
