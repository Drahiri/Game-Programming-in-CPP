#ifndef RENDERER_H
#define RENDERER_H

#include "math.h"

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

class Game;
class Mesh;
class MeshComponent;
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

    void addMeshComp(MeshComponent* meshComp);
    void removeMeshComp(MeshComponent* meshComp);

    Texture* getTexture(const std::string& fileName);
    Mesh* getMesh(const std::string& fileName);

    float getScreenWidth() const;
    float getScreenHeight() const;

private:
    bool loadShaders();
    void createSpriteVerts();

    float screenWidth;
    float screenHeight;

    std::unordered_map<std::string, Texture*> textures;

    // View/Projection matrices
    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;

    // Sprite stuff
    std::vector<SpriteComponent*> sprites;
    Shader* spriteShader;
    VertexArray* spriteVerts;

    // Mesh stuff
    std::unordered_map<std::string, Mesh*> meshes;
    Shader* meshShader;
    std::vector<MeshComponent*> meshComps;

    Game* game;
    SDL_Window* window;
    SDL_GLContext context;
};

#endif