#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

class Game;
class Mesh;
class Shader;
class SpriteComponent;
class Texture;
class VertexArray;

class Renderer {
public:
    Renderer(Game* game);
    ~Renderer();

    // Initialize and shutdown renderer
    bool initialize(float windowWidth, float windowHeight);
    void shutdown();

    // Unload all loaded textures/meshes
    void unloadData();

    // Draw the frame
    void draw();

    void addSprite(SpriteComponent* sprite);
    void removeSprite(SpriteComponent* sprite);

    Texture* getTexture(const std::string& fileName);
    Mesh* getMesh(const std::string& fileName);

    float getScreenWidth() const;
    float getScreenHeight() const;

private:
    bool loadShaders();
    void createSpriteVerts();

    float screenWidth;
    float screenHeight;

    std::vector<SpriteComponent*> sprites;
    std::unordered_map<std::string, Texture*> textures;
    std::unordered_map<std::string, Mesh*> meshes;

    Shader* spriteShader;
    VertexArray* spriteVerts;

    Game* game;
    SDL_Window* window;
    SDL_GLContext context;
};

#endif