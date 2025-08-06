#ifndef ANIMATION_H
#define ANIMATION_H

#include "bone_transform.h"
#include "math.h"

#include <string>
#include <vector>

class Skeleton;

class Animation {
public:
    bool load(const std::string& fileName);

    size_t getNumBones() const;
    size_t getNumFrames() const;

    float getDuration() const;
    float getFrameDuration() const;
    const std::string& getFileName() const;

    // Fills the provided vector with the global (current) pose matrices
    // for each bones at the specific time in the animation.
    void getGlobalPoseAtTime(
          std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const;

    bool loadBinary(const std::string& fileName);
    void saveBinary(const std::string& fileName,
          size_t frames,
          float length,
          size_t numBones,
          std::vector<std::vector<BoneTransform>>& tracks);

private:
    // Number of bones for the animation
    size_t numBones;
    // Number of flames in the animation
    size_t numFrames;
    // Duration of the animation in secons
    float duration;
    // Duration of each frame in animation
    float frameDuration;
    // Transform information for each frame on the track
    // Each index in the outer vector is a bone, inner vector is a frame
    std::vector<std::vector<BoneTransform>> tracks;

    std::string fileName;
};

#endif