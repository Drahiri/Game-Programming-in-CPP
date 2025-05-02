#ifndef GAME_H
#define GAME_H

#include <map>
#include <SDL3/SDL.h>
#include <string>
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

    SDL_Texture* getTexture(const std::string& filename);

    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;

    // Time keeping
    Uint32 ticksCount;

    // Objects
    bool updatingActors;
    std::vector<Actor*> actors;
    std::vector<Actor*> pendingActors;

    // Textures
    std::map<std::string, SDL_Texture*> textures;
};

#endif