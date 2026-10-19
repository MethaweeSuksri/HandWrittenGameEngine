#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

static SDL_Window *mainWindow = nullptr;
static SDL_Renderer *mainRenderer = nullptr;
static SDL_Surface *surface = nullptr;
static SDL_Texture *texture = nullptr;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv){

    // Set Priority for all category 
    SDL_SetLogPriority( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG );

    if ( !SDL_Init( SDL_INIT_VIDEO ) ){
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Fail to initialize SDL video subsystem: %s", SDL_GetError());
        return  SDL_APP_FAILURE;
    }

    if ( !SDL_CreateWindowAndRenderer(
            "Hello World",
            800, 600,
            SDL_WINDOW_RESIZABLE,
            &mainWindow,
            &mainRenderer ) ) {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Fail to create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    surface = SDL_LoadPNG("./build/ME.png");
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    texture = SDL_CreateTextureFromSurface(mainRenderer, surface);

    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(surface);
    
    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate){
    
    SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(mainRenderer);
    SDL_RenderTexture(mainRenderer, texture, NULL, NULL);
    SDL_RenderPresent(mainRenderer);
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    
    switch( event->type ){
        case SDL_EVENT_QUIT:
            SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, " EVENT QUIT " );

            return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){

    SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Quitting..... " );

    return;
}