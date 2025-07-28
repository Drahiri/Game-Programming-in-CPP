#include "skeletal_mesh_component.h"

#include "actor.h"
#include "animation.h"
#include "game.h"
#include "skeleton.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner) :
    MeshComponent(owner, true),
    skeleton(nullptr),
    animation(nullptr),
    animPlayRate(1.0f),
    animTime(0.0f) {}

void SkeletalMeshComponent::draw(Shader* shader) {
    MeshComponent::draw(shader);
}

void SkeletalMeshComponent::computeMatrixPalette() {
    const std::vector<Matrix4>& globalInvBindPoses = skeleton->getGlobalInvBindPoses();
    std::vector<Matrix4> currentPoses;
    animation->getGlobalPoseAtTime(currentPoses, skeleton, animTime);

    // Setup the palette for each bone
    for(size_t i = 0; i < skeleton->getNumBones(); i++) {
        // Global inverse bind pose matrix times current pose
        palette.entry[i] = globalInvBindPoses[i] * currentPoses[i];
    }
}

void SkeletalMeshComponent::setSkeleton(const Skeleton* sk) {
    skeleton = sk;
}