#include "sdl_gui.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* myRenderer = NULL;
SDL_Surface *OneC;
SDL_Surface *TwoC;
SDL_Surface *ThreeC;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} cardFrame;

cardFrame C1, C2, C3, C4, C5, C6, C7, F1, F2, F3, F4;

int initialize_window(void) {

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Yukon",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);

    myRenderer = SDL_CreateRenderer(window, -1, 0);

    return TRUE;
}

void setup_card_frames(void) {
    C1.x = 30;
    C2.x = C1.x + 130;
    C3.x = C2.x + 130;
    C4.x = C3.x + 130;
    C5.x = C4.x + 130;
    C6.x = C5.x + 130;
    C7.x = C6.x + 130;
    C1.y = C2.y = C3.y = C4.y = C5.y = C6.y = C7.y = 30;
    F1.x = F2.x = F3.x = F4.x = C7.x + 150;
    F1.y = 200;
    F2.y = F1.y + 130;
    F3.y = F2.y + 130;
    F4.y = F3.y + 130;
    C1.width = C2.width = C3.width = C4.width = C5.width = C6.width = C7.width = F1.width = F2.width = F3.width = F4.width = CARD_WIDTH;
    C1.height = C2.height = C3.height = C4.height = C5.height = C6.height = C7.height = F1.height = F2.height = F3.height = F4.height = CARD_HEIGHT;
}

void render(void) {
    SDL_SetRenderDrawColor(myRenderer, 0, 80, 0, 255);
    SDL_RenderClear(myRenderer);

    SDL_Rect C1_rect = {C1.x, C1.y, C1.width, C1.height};
    SDL_Rect C2_rect = {C2.x, C2.y, C2.width, C2.height};
    SDL_Rect C3_rect = {C3.x, C3.y, C3.width, C3.height};
    SDL_Rect C4_rect = {C4.x, C4.y, C4.width, C4.height};
    SDL_Rect C5_rect = {C5.x, C5.y, C5.width, C5.height};
    SDL_Rect C6_rect = {C6.x, C6.y, C6.width, C6.height};
    SDL_Rect C7_rect = {C7.x, C7.y, C7.width, C7.height};
    SDL_Rect F1_rect = {F1.x, F1.y, F1.width, F1.height};
    SDL_Rect F2_rect = {F2.x, F2.y, F2.width, F2.height};
    SDL_Rect F3_rect = {F3.x, F3.y, F3.width, F3.height};
    SDL_Rect F4_rect = {F4.x, F4.y, F4.width, F4.height};

    SDL_SetRenderDrawColor(myRenderer, 50, 50, 50, 255);

    SDL_RenderDrawRect(myRenderer, &C1_rect);
    SDL_RenderPresent(myRenderer);
    SDL_Delay(dealDelay);
    SDL_RenderDrawRect(myRenderer, &C2_rect);
    SDL_RenderPresent(myRenderer);
    SDL_Delay(dealDelay);
    SDL_RenderDrawRect(myRenderer, &C3_rect);
    SDL_RenderPresent(myRenderer);
    SDL_Delay(dealDelay);
    SDL_RenderDrawRect(myRenderer, &C4_rect);
    SDL_RenderPresent(myRenderer);
    SDL_Delay(dealDelay);
    SDL_RenderDrawRect(myRenderer, &C5_rect);
    SDL_RenderPresent(myRenderer);
    SDL_Delay(dealDelay);
    SDL_RenderDrawRect(myRenderer, &C6_rect);
    SDL_RenderPresent(myRenderer);
    SDL_Delay(dealDelay);
    SDL_RenderDrawRect(myRenderer, &C7_rect);
    SDL_RenderPresent(myRenderer);
    SDL_Delay(dealDelay);
    SDL_RenderDrawRect(myRenderer, &F1_rect);
    SDL_RenderPresent(myRenderer);
    SDL_Delay(dealDelay);
    SDL_RenderDrawRect(myRenderer, &F2_rect);
    SDL_RenderPresent(myRenderer);
    SDL_Delay(dealDelay);
    SDL_RenderDrawRect(myRenderer, &F3_rect);
    SDL_RenderPresent(myRenderer);
    SDL_Delay(dealDelay);
    SDL_RenderDrawRect(myRenderer, &F4_rect);

    int imgFlags = IMG_INIT_PNG;
    int initStatus = IMG_Init(imgFlags);
    if((initStatus & imgFlags) != imgFlags) {
        printf("SDL_Image format not available.");
    }

    OneC = IMG_Load("images/cards_pack/PNG/Medium/1C.png");
    TwoC = IMG_Load("images/cards_pack/PNG/Medium/2C.png");
    ThreeC = IMG_Load("images/cards_pack/PNG/Medium/3C.png");

    SDL_Texture* AC = SDL_CreateTextureFromSurface(myRenderer, OneC);

    SDL_RenderCopy(myRenderer, AC, NULL, &C1_rect);

    SDL_RenderPresent(myRenderer);
}

void process_input(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT], SDL_Renderer *renderer, const char* path) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            game_is_running = FALSE;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    game_is_running = FALSE;
                    break;
                case SDLK_l:
                    //load_SDL_cards(columns, deck);
                    //load_card_texture(renderer, path);
                    //load_card_front_images(renderer, *deck);
                    //load_card_texture(,)
                    break;
            }
            break;
    }
}

void destroy_window(void) {
    //SDL_FreeSurface(OneC);
    //SDL_FreeSurface(TwoC);
    //SDL_FreeSurface(ThreeC);
    SDL_DestroyRenderer(myRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* load_card_texture(SDL_Renderer *renderer, const char *path) {
    int imgFlags = IMG_INIT_PNG;
    int initStatus = IMG_Init(imgFlags);
    if((initStatus & imgFlags) != imgFlags) {
        printf("SDL_Image format not available.");
    }

    SDL_Surface *surface = IMG_Load(path);

    if(!surface) {
        printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

void load_card_front_images(SDL_Renderer *renderer, struct card_llist *deck) {
    while (deck != NULL) {
        char path[40];
        sprintf(path, "images/cards_pack/PNG/Medium/%d%d.png", deck->value, deck->suit);
        deck = deck->next;
    }
}

void *load_SDL_cards(struct card_llist *columns[COLUMNS], struct card_llist *deck[CARD_COUNT])
{
    for (int i = 0; i < COLUMNS; i++)
        remove_cards(columns[i]);

    int result;

    const char cards[CARD_COUNT][2] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC",
                                       "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD",
                                       "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH",
                                       "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS"};
    result = load_cards_from_array(deck, cards);

    if (result != 0)
        return "Error loading cards";

    result = deck_to_columns(columns, deck);
    if (result != 0)
        return "Error moving cards to columns";

    return 0;
}

/*
char *load_card_images(struct card_llist *deck[CARD_COUNT]) {
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return "SDL_image initialization failed";
    }
    for (int i = 0; i < CARD_COUNT; i++) {
        char path[40];
        sprintf(path, "images/cards_pack/PNG/Medium/%d%d.PNG", deck[i]->value, deck[i]->suit);

        SDL_Surface *surface = IMG_Load(path);
        if(!surface) {
            printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
            return "Error loading card images";
        }

        deck[i]->front_image = SDL_CreateTextureFromSurface(myRenderer, surface);
        SDL_FreeSurface(surface);

        if (!deck[i]->front_image) {
            printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
            return "Error creating card textures";
        }
    }
    return NULL;
}*/