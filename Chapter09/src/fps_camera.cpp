#include "fps_camera.h"

#include "actor.h"

FPSCamera::FPSCamera(Actor* owner) :
    CameraComponent(owner),
    pitchSpeed(0.0f),
    maxPitch(Math::ToRadians(60)),
    pitch(0.0f) {}

void FPSCamera::update(float deltaTime) {
    CameraComponent::update(deltaTime);
    // Camera position is owner position
    Vector3 cameraPos = owner->getPosition();

    // Update pitch based on pitch speed
    pitch += pitchSpeed * deltaTime;
    // Clamp pitch to [-max, +max]
    pitch = Math::Clamp(pitch, -maxPitch, maxPitch);
    // Make quaternion representing pitch rotation,
    // which is about owner's right vector
    Quaternion q(owner->getRight(), pitch);

    // Rotate owner forward by pitch quaternion
    Vector3 viewForward = Vector3::Transform(owner->getForward(), q);
    // Target position 100 units in front of owner
    Vector3 target = cameraPos + viewForward * 100;
    // Also rotate up by pitch quaternion
    Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

    // Create look at matrix, set as view
    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);

    setViewMatrix(view);
}

float FPSCamera::getPitchSpeed() const {
    return pitchSpeed;
}

void FPSCamera::setPitchSpeed(float speed) {
    pitchSpeed = speed;
}

float FPSCamera::getMaxPitch() const {
    return maxPitch;
}

void FPSCamera::setMaxPitch(float pitch) {
    maxPitch = pitch;
}

float FPSCamera::getPitch() const {
    return pitch;
}