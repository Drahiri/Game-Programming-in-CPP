#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

class Game;
class Texture;
class VertexArray;
class Renderer;

class Mesh {
public:
    Mesh();
    ~Mesh();

    // Load/unload mesh
    bool load(const std::string& fileName, Renderer* renderer);
    void unload();

    // Get the vertex array associated with this mesh
    VertexArray* getVertexArray();

    // Get a texture from specified index
    Texture* getTexture(size_t index);

    // Get name of shader
    const std::string& getShaderName() const;

    // Get object space bounding sphere radius
    float getRadius() const;

private:
    // Textures associated with this mesh
    std::vector<Texture*> textures;

    // Vertex array associated with this mesh
    VertexArray* vertexArray;

    // Name of shader specified by mesh
    std::string shaderName;

    // Stores object space bounding sphere radius
    float radius;
};

#endif