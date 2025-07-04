#include "orbit_camera.h"

#include "actor.h"

OrbitCamera::OrbitCamera(Actor* owner) :
    CameraComponent(owner),
    offset(Vector3(-400.0f, 0.0f, 0.0f)),
    up(Vector3::UnitZ),
    pitchSpeed(0.0f),
    yawSpeed(0.0f) {}

void OrbitCamera::update(float deltaTime) {
    CameraComponent::update(deltaTime);

    // Create a quaternion for yaw about world up
    Quaternion yaw(Vector3::UnitZ, yawSpeed * deltaTime);
    // Transform offset and up by yaw
    offset = Vector3::Transform(offset, yaw);
    up = Vector3::Transform(up, yaw);

    // Compute camera forward/right from these vectors
    // Forward owner.position - (owner.position + offset)
    // = -offset
    Vector3 forward = -1.0f * offset;
    forward.Normalize();
    Vector3 right = Vector3::Cross(up, forward);
    right.Normalize();

    // Create quaternion for pitch about camera right
    Quaternion pitch(right, pitchSpeed * deltaTime);
    // Transform camera offset and up by pitch
    offset = Vector3::Transform(offset, pitch);
    up = Vector3::Transform(up, pitch);

    // Compute transform matrix
    Vector3 target = owner->getPosition();
    Vector3 cameraPos = target + offset;
    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    setViewMatrix(view);
}

float OrbitCamera::getPitchSpeed() const {
    return pitchSpeed;
}

void OrbitCamera::setPitchSpeed(float speed) {
    pitchSpeed = speed;
}

float OrbitCamera::getYawSpeed() const {
    return yawSpeed;
}

void OrbitCamera::setYawSpeed(float speed) {
    yawSpeed = speed;
}