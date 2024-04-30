#include <SDL.h>
#include <SDL_image.h>
#include "sdl_defines.h"
#include "../../yukon_commands.h"

extern int game_is_running;
extern SDL_Window* window;
extern SDL_Renderer* myRenderer;

int initialize_window();
void setup_card_frames();
void render();
void process_input(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT], SDL_Renderer *renderer, const char* path);
void destroy_window();
SDL_Texture* load_card_texture(SDL_Renderer *renderer, const char *path);
void load_card_front_images(SDL_Renderer *renderer, struct card_llist *deck);
void *load_SDL_cards(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT]);