#ifndef MESH_H
#define MESH_H

#include "collision.h"

#include <string>
#include <vector>

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

    // Get object spec powere
    float getSpecPower() const;

    const AABB& getBox() const;

private:
    // Textures associated with this mesh
    std::vector<Texture*> textures;

    // Vertex array associated with this mesh
    VertexArray* vertexArray;

    // Name of shader specified by mesh
    std::string shaderName;

    // Stores object space bounding sphere radius
    float radius;

    // Spec power
    float specPower;

    // AABB collision
    AABB box;
};

#endif