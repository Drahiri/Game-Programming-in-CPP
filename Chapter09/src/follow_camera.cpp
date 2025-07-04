#include "follow_camera.h"

#include "actor.h"

FollowCamera::FollowCamera(Actor* owner) :
    CameraComponent(owner),
    horzDist(350.f),
    vertDist(150.0f),
    targetDist(100.0f) {}

void FollowCamera::update(float deltaTime) {
    CameraComponent::update(deltaTime);

    // Target is target dist in front of owning actor
    Vector3 target = owner->getPosition() + owner->getForward() * targetDist;

    // Up is just UnitZ since we don't flip the camera
    Matrix4 view = Matrix4::CreateLookAt(computeCameraPos(), target, Vector3::UnitZ);
    setViewMatrix(view);
}

Vector3 FollowCamera::computeCameraPos() const {
    // Set camera position behind and above owner
    Vector3 cameraPos = owner->getPosition();
    cameraPos -= owner->getForward() * horzDist;
    cameraPos += Vector3::UnitZ * vertDist;

    return cameraPos;
}

void FollowCamera::setHorzDist(float dist) {
    horzDist = dist;
}

void FollowCamera::setVertDist(float dist) {
    vertDist = dist;
}

void FollowCamera::setTargetDist(float dist) {
    targetDist = dist;
}
