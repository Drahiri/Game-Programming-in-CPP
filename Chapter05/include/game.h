#ifndef GAME_H
#define GAME_H

#include "math.h"

#include <map>
#include <SDL3/SDL.h>
#include <string>
#include <vector>

class Actor;
class SpriteComponent;
class Ship;
class Asteroid;
class VertexArray;

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

    std::vector<Asteroid*>& getAsteroids();

    void addAsteroid(Asteroid* ast);
    void removeAsteroid(Asteroid* ast);

private:
    // Helper functions for the game loop
    void processInput();
    void updateGame();
    void generateOutput();

    void loadData();
    void unloadData();

    void initSpriteVerts();

    bool isRunning;
    SDL_Window* window;
    SDL_GLContext context;

    // Time keeping
    Uint32 ticksCount;

    // Objects
    bool updatingActors;
    std::vector<Actor*> actors;
    std::vector<Actor*> pendingActors;

    Ship* ship;

    // Textures
    std::map<std::string, SDL_Texture*> textures;

    // Sprites
    std::vector<SpriteComponent*> sprites;

    // Asteroids
    std::vector<Asteroid*> asteroids;

    // Vertices
    VertexArray* spriteVerts;
};

#endif