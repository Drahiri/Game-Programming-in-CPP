#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <vector>

struct Vector2 {
    float x;
    float y;
};

struct Ball {
    Vector2 position;
    Vector2 velocity;
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
    std::vector<Ball> balls;

    // Player 1
    Vector2 paddlePos1;
    int paddleDir1;
    // Player2
    Vector2 paddlePos2;
    int paddleDir2;
};

#endif