#include "skeleton.h"

#include <fstream>
#include <rapidjson/document.h>
#include <SDL3/SDL_log.h>
#include <sstream>

bool Skeleton::load(const std::string& fileName) {
    return true;
}

void Skeleton::computeGlobalInvBindPose() {}

size_t Skeleton::getNumBones() const {
    return bones.size();
}

const Skeleton::Bone& Skeleton::getBone(size_t idx) const {
    return bones[idx];
}

const std::vector<Skeleton::Bone>& Skeleton::getBones() const {
    return bones;
}

const std::vector<Matrix4>& Skeleton::getGlobalInvBindPoses() const {
    return globalInvBindPoses;
}
