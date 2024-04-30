#include "sdl_gui.h"

int main(int argc, char* argv[]) {

    game_is_running = initialize_window();
    setup_card_frames();
    render();

    struct card_llist *deck[CARD_COUNT];
    struct card_llist *columns[COLUMNS];
    struct card_llist *foundations[FOUNDATIONS];
    for (int i = 0; i < CARD_COUNT; i++)
        deck[i] = NULL;
    for (int i = 0; i < COLUMNS; i++)
        columns[i] = NULL;
    for (int i = 0; i < FOUNDATIONS; i++)
        foundations[i] = NULL;

    while(game_is_running) {
        process_input(columns,deck,myRenderer,NULL);
    }

    destroy_window();

    return 0;
}