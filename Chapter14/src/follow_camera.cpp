#include "follow_camera.h"

#include "actor.h"
#include "level_loader.h"

FollowCamera::FollowCamera(Actor* owner) :
    CameraComponent(owner),
    horzDist(350.f),
    vertDist(150.0f),
    targetDist(100.0f),
    springConstant(128.0f) {}

void FollowCamera::update(float deltaTime) {
    CameraComponent::update(deltaTime);

    // Compute dampening from sprint constant
    float dampening = 2.0f * Math::Sqrt(springConstant);

    // Compute ideal position
    Vector3 idealPos = computeCameraPos();

    // Compute difference between actual and ideal
    Vector3 diff = actualPos - idealPos;
    // Compute acceleration of spring
    Vector3 accel = -springConstant * diff - dampening * velocity;

    // Update velocity
    velocity += accel * deltaTime;
    // Update actual camera position
    actualPos += velocity * deltaTime;

    // Target is target dist in front of owning actor
    Vector3 target = owner->getPosition() + owner->getForward() * targetDist;

    // Up is just UnitZ since we don't flip the camera
    // Use actual position here, not ideal
    Matrix4 view = Matrix4::CreateLookAt(actualPos, target, Vector3::UnitZ);
    setViewMatrix(view);
}

Vector3 FollowCamera::computeCameraPos() const {
    // Set camera position behind and above owner
    Vector3 cameraPos = owner->getPosition();
    cameraPos -= owner->getForward() * horzDist;
    cameraPos += Vector3::UnitZ * vertDist;

    return cameraPos;
}

void FollowCamera::snapToIdeal() {
    // Set actual position to ideal
    actualPos = computeCameraPos();
    // Zero velocity
    velocity = Vector3::Zero;
    // Compute target and view
    Vector3 target = owner->getPosition() + owner->getForward() * targetDist;
    Matrix4 view = Matrix4::CreateLookAt(actualPos, target, Vector3::UnitZ);
    setViewMatrix(view);
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

void FollowCamera::setSpringConstant(float spring) {
    springConstant = spring;
}

Component::TypeID FollowCamera::getType() const {
    return Component::TypeID::FollowCamera;
}

void FollowCamera::loadProperties(const rapidjson::Value& inObject) {
    CameraComponent::loadProperties(inObject);

    JsonHelper::getVector3(inObject, "actualPos", actualPos);
    JsonHelper::getVector3(inObject, "velocity", velocity);
    JsonHelper::getFloat(inObject, "horzDist", horzDist);
    JsonHelper::getFloat(inObject, "vertDist", vertDist);
    JsonHelper::getFloat(inObject, "targetDist", targetDist);
    JsonHelper::getFloat(inObject, "springConstant", springConstant);
}

void FollowCamera::saveProperties(
      rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject) const {
    CameraComponent::saveProperties(alloc, inObject);

    JsonHelper::addVector3(alloc, inObject, "actualPos", actualPos);
    JsonHelper::addVector3(alloc, inObject, "velocity", velocity);
    JsonHelper::addFloat(alloc, inObject, "horzDist", horzDist);
    JsonHelper::addFloat(alloc, inObject, "vertDist", vertDist);
    JsonHelper::addFloat(alloc, inObject, "targetDist", targetDist);
    JsonHelper::addFloat(alloc, inObject, "springConstant", springConstant);
}
