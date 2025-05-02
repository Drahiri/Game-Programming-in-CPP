#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <vector>

class Actor;

class Game {
public:
    Game();

    bool initialize();
    void runLoop();
    void shutdown();

    void addActor(Actor* actor);
    void removeActor(Actor* actor);

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

    // Objects
    bool updatingActors;
    std::vector<Actor*> actors;
    std::vector<Actor*> pendingActors;
};

#endif