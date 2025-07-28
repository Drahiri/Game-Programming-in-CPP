#include "skeletal_mesh_component.h"

#include "actor.h"
#include "game.h"
#include "skeleton.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner) :
    MeshComponent(owner, true),
    skeleton(nullptr) {}

void SkeletalMeshComponent::draw(Shader* shader) {
    MeshComponent::draw(shader);
}

void SkeletalMeshComponent::setSkeleton(Skeleton* sk) {
    skeleton = sk;
}