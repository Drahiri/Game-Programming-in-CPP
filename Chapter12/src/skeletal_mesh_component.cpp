#include "skeletal_mesh_component.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner) : MeshComponent(owner) {
    isSkeletal = true;
}

void SkeletalMeshComponent::draw(Shader* shader) {
    MeshComponent::draw(shader);
}
