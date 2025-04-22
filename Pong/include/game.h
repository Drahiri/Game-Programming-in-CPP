#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>

struct Vector2 {
    float x;
    float y;
};

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

    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;

    // Time keeping
    Uint32 ticksCount;

    // Game objects
    Vector2 paddlePos;
    Vector2 ballPos;
};

#endif