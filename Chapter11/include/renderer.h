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

struct DirectionalLight {
    // Direction of light
    Vector3 direction;
    // Diffuse Color
    Vector3 diffuseColor;
    // Specular color;
    Vector3 specColor;
};

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

    void setViewMatrix(const Matrix4& view);

    void setAmbientLight(const Vector3& light);

    DirectionalLight& getDirectionalLight();

    // Setting light uniforms
    void setLightUniforms(Shader* shader);

    // Unprojection
    Vector3 unproject(const Vector3& screenPoint) const;
    // For picking
    void getScreenDirection(Vector3& outStart, Vector3& outDir) const;

    void setRelativeMouse(bool relative);

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

    // Lights
    Vector3 ambientLight;
    DirectionalLight dirLight;
};

#endif