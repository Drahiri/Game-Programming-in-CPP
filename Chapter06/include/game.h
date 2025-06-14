#ifndef GAME_H
#define GAME_H

#include "math.h"

#include <map>
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

class Actor;
class Mesh;
class Shader;
class SpriteComponent;
class Texture;
class VertexArray;

class Game {
public:
    Game();

    bool initialize();
    void runLoop();
    void shutdown();

    void addActor(Actor* actor);
    void removeActor(Actor* actor);

    Texture* getTexture(const std::string& fileName);
    void addSprite(SpriteComponent* sprite);
    void removeSprite(SpriteComponent* sprite);

    Vector2 getScreenSize() const;

    Mesh* getMesh(const std::string& fileName);

private:
    // Helper functions for the game loop
    void processInput();
    void updateGame();
    void generateOutput();

    void loadData();
    void unloadData();

    void initSpriteVerts();
    bool loadShaders();

    bool isRunning;
    SDL_Window* window;
    SDL_GLContext context;

    // Time keeping
    Uint32 ticksCount;

    // Objects
    bool updatingActors;
    std::vector<Actor*> actors;
    std::vector<Actor*> pendingActors;

    // Textures
    std::map<std::string, Texture*> textures;

    // Sprites
    std::vector<SpriteComponent*> sprites;

    // Vertices
    VertexArray* spriteVerts;

    // Shaders
    Shader* spriteShader;

    // Meshes
    std::unordered_map<std::string, Mesh*> meshes;
};

#endif