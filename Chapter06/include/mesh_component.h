#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "component.h"

#include <cstddef>

class Actor;
class Shader;
class Mesh;

class MeshComponent : public Component {
public:
    MeshComponent(Actor* owner);
    ~MeshComponent();

    // Draw this mesh component with provided shader
    virtual void draw(Shader* shader);

    // Set the mesh/texture index used by mesh component
    virtual void setMesh(Mesh* mesh);
    Mesh* getMesh() const;

    void setTextureIndex(size_t index);

protected:
    Mesh* mesh;
    size_t textureIndex;
};

#endif