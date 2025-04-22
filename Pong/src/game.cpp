#include "game.h"

Game::Game() : window(nullptr), isRunning(true) {}

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
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void Game::processInput() {}

void Game::updateGame() {}

void Game::generateOutput() {}
