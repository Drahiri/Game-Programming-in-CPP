#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>

class Game {
public:
    Game();

    bool initialize();
    void runLoop();
    void shutdown();

private:
    // Helper functions for the game loop
    void processInput();
    void updateGame();
    void generateOutput();

    SDL_Window* window;
    bool isRunning;
};

#endif