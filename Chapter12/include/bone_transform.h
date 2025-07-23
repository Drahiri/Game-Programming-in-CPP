#ifndef BONE_TRANSFORM_H
#define BONE_TRANSFORM_H

#include "math.h"

struct BoneTransform {
    Quaternion rotation;
    Vector3 translation;

    Matrix4 toMatrix() const;

    static BoneTransform interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};

#endif