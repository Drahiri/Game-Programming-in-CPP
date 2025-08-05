#ifndef MESH_H
#define MESH_H

#include "collision.h"
#include "vertex_array.h"

#include <cstdint>
#include <string>
#include <vector>

class Texture;
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
    // Get file name
    const std::string& getFileName() const;

    // Get object space bounding sphere radius
    float getRadius() const;

    // Get object spec powere
    float getSpecPower() const;

    const AABB& getBox() const;

    void saveBinary(const std::string& fileName,
          const void* verts,
          uint32_t numVerts,
          VertexArray::Layout layout,
          const uint32_t* indices,
          uint32_t numIndices,
          const std::vector<std::string>& textureNames,
          const AABB& box,
          float radius);

private:
    // Textures associated with this mesh
    std::vector<Texture*> textures;

    // Vertex array associated with this mesh
    VertexArray* vertexArray;

    // Name of shader specified by mesh
    std::string shaderName;

    // Mesh file name
    std::string fileName;

    // Stores object space bounding sphere radius
    float radius;

    // Spec power
    float specPower;

    // AABB collision
    AABB box;
};

#endif