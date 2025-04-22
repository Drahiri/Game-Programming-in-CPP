#include "game.h"

Game::Game() : isRunning(true), window(nullptr), renderer(nullptr) {}

bool Game::initialize() {
    // Initialize SDL
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Creating Window
    window = SDL_CreateWindow("Game Programming - Pong", 1024, 768, 0);
    if(!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Creating Renderer
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);
    SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);

    renderer = SDL_CreateRendererWithProperties(props);
    if(!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

void Game::runLoop() {
    while(isRunning) {
        processInput();
        updateGame();
        generateOutput();
    }
}

void Game::shutdown() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void Game::processInput() {
    // Process SDL events
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;

        default:
            break;
        }
    }

    // Process keyboard state
    const bool* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]) {
        isRunning = false;
    }
}

void Game::updateGame() {}

void Game::generateOutput() {}
