#ifndef GAME_H
#define GAME_H

#include "math.h"

#include <map>
#include <SDL3/SDL.h>
#include <string>
#include <vector>

class Actor;
class SpriteComponent;
class Enemy;
class Grid;

class Game {
public:
    Game();

    bool initialize();
    void runLoop();
    void shutdown();

    void addActor(Actor* actor);
    void removeActor(Actor* actor);

    SDL_Texture* getTexture(const std::string& filename);
    void addSprite(SpriteComponent* sprite);
    void removeSprite(SpriteComponent* sprite);

    Vector2 getScreenSize() const;

    Grid* getGrid();
    std::vector<Enemy*>& getEnemies();
    Enemy* getNearestEnemy(const Vector2& pos);

private:
    // Helper functions for the game loop
    void processInput();
    void updateGame();
    void generateOutput();

    void loadData();
    void unloadData();

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

    // Sprites
    std::vector<SpriteComponent*> sprites;

    // Game-specific
    std::vector<Enemy*> enemies;
    Grid* grid;
    float nextEnemy;
};

#endif