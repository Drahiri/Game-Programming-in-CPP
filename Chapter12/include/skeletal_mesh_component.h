#ifndef SKELETAL_MESH_COMPONENT_H
#define SKELETAL_MESH_COMPONENT_H

#include "matrix_palette.h"
#include "mesh_component.h"

class Actor;
class Animation;
class Skeleton;
class Shader;

class SkeletalMeshComponent : public MeshComponent {
public:
    SkeletalMeshComponent(Actor* owner);

    // Draw this mesh component
    void draw(Shader* shader) override;

    void computeMatrixPalette();
    void setSkeleton(const Skeleton* sk);

private:
    const Skeleton* skeleton;

    // Matrix palette
    MatrixPalette palette;
    // Animation currently playing
    const Animation* animation;
    // Play rate of animation (1.0 is normal speed)
    float animPlayRate;
    // Current time in the animation
    float animTime;
};

#endif