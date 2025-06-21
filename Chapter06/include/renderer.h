#ifndef RENDERER_H
#define RENDERER_H

#include "math.h"

#include <array>
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
    // Diffuse color
    Vector3 diffuseColor;
    // Specular color;
    Vector3 specColor;
};

struct PointLight {
    // Position
    Vector3 position;
    // Diffuse color
    Vector3 diffuseColor;
    // Specular color
    Vector3 specColor;
    // Affect radius
    float radius;
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

    Shader* getMeshShader(const std::string& shaderName);

    float getScreenWidth() const;
    float getScreenHeight() const;

    void setAmbientLight(const Vector3& light);

    DirectionalLight& getDirectionalLight();
    std::array<PointLight, 4>& getPointLights();

    // Setting light uniforms
    void setLightUniforms(Shader* shader);

    void addShaderMeshComp(MeshComponent* meshComp);
    void removeShaderMeshComp(MeshComponent* meshComp);

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
    std::unordered_map<std::string, Shader*> meshShaders;
    std::vector<MeshComponent*> meshComps;
    std::unordered_map<std::string, std::vector<MeshComponent*>> shaderMeshComps;

    Game* game;
    SDL_Window* window;
    SDL_GLContext context;

    // Lights
    Vector3 ambientLight;
    DirectionalLight dirLight;
    std::array<PointLight, 4> pointLights;
};

#endif