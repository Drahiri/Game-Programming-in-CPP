#ifndef RENDERER_H
#define RENDERER_H

#include <string>

class Mesh;
class SpriteComponent;
class Texture;

class Renderer {
public:
    Renderer();
    ~Renderer();

    // Initialize and shutdown renderer
    bool initialize(float screenWidth, float screenHeight);
    void shutdown();

    // Unload all loaded textures/meshes
    void unloadData();

    // Draw the frame
    void draw();

    void addSprite(SpriteComponent* sprite);
    void removeSprite(SpriteComponent* sprite);
    Texture* getTexture(const std::string& fileName);
    Mesh* getMesh(const std::string& fileName);

private:
    bool loadShaders();
    void createSpriteVerts();
};

#endif