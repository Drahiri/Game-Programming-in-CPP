#include "bone_transform.h"

Matrix4 BoneTransform::toMatrix() const {
    Matrix4 rot = Matrix4::CreateFromQuaternion(rotation);
    Matrix4 trans = Matrix4::CreateTranslation(translation);

    return rot * trans;
}

BoneTransform BoneTransform::interpolate(const BoneTransform& a, const BoneTransform& b, float f) {
    BoneTransform retVal;
    retVal.rotation = Quaternion::Slerp(a.rotation, b.rotation, f);
    retVal.translation = Vector3::Lerp(a.translation, b.translation, f);

    return retVal;
}
