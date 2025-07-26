#ifndef SKELETAL_MESH_COMPONENT_H
#define SKELETAL_MESH_COMPONENT_H

#include "mesh_component.h"

class Actor;
class Shader;

class SkeletalMeshComponent : public MeshComponent {
public:
    SkeletalMeshComponent(Actor* owner);

    // Draw this mesh component
    void draw(Shader* shader) override;
};

#endif