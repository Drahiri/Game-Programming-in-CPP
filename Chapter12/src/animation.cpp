#include "animation.h"

#include "skeleton.h"

bool Animation::load(const std::string& fileName) {}

size_t Animation::getNumBones() const {
    return numBones;
}

size_t Animation::getNumFrames() const {
    return numFrames;
}

float Animation::getDuration() const {
    return duration;
}

float Animation::getFrameDuration() const {
    return frameDuration;
}

void Animation::getGlobalPoseAtTime(
      std::vector<Matrix4>& outPose, Skeleton* inSkeleton, float inTime) const {}