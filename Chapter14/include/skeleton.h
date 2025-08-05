#ifndef SKELETON_H
#define SKELETON_H

#include "bone_transform.h"
#include "math.h"

#include <string>
#include <vector>

class Skeleton {
public:
    // Definition for each bone in the skeleton
    struct Bone {
        BoneTransform localBindPose;
        std::string name;
        int parent;
    };

    // Load from file
    bool load(const std::string& fileName);

    // Getter functions
    size_t getNumBones() const;
    const Bone& getBone(size_t idx) const;
    const std::vector<Bone>& getBones() const;
    const std::vector<Matrix4>& getGlobalInvBindPoses() const;
    const std::string& getFileName() const;

protected:
    // Compute the global inverse bind pose for each bone
    // Called when loading skeleton
    void computeGlobalInvBindPose();

private:
    // The bones in the skeleton
    std::vector<Bone> bones;
    // The global inverse bind poses for each bone
    std::vector<Matrix4> globalInvBindPoses;

    std::string fileName;
};

#endif