#include "bone_transform.h"

Matrix4 BoneTransform::toMatrix() const {
    Matrix4 rot = Matrix4::CreateFromQuaternion(rotation);
    Matrix4 trans = Matrix4::CreateTranslation(translation);

    return rot * trans;
}