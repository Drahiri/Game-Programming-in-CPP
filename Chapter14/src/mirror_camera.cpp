#include "mirror_camera.h"

#include "actor.h"
#include "game.h"
#include "level_loader.h"
#include "renderer.h"

MirrorCamera::MirrorCamera(Actor* owner) :
    CameraComponent(owner),
    horzDist(150.0f),
    vertDist(200.0f),
    targetDist(400.0f) {}

void MirrorCamera::update(float deltaTime) {
    CameraComponent::update(deltaTime);
    Vector3 idealPos = computeCameraPos();

    // Target is target dist in front of owning actor
    Vector3 target = owner->getPosition() - owner->getForward() * targetDist;

    // Use ideal position here
    Matrix4 view = Matrix4::CreateLookAt(idealPos, target, Vector3::UnitZ);
    Game* game = owner->getGame();
    game->getRenderer()->setMirrorView(view);
}

void MirrorCamera::snapToIdeal() {
    // Set actual position to ideal
    Vector3 idealPos = computeCameraPos();
    // Compute target and view
    Vector3 target = owner->getPosition() - owner->getForward() * targetDist;

    Matrix4 view = Matrix4::CreateLookAt(idealPos, target, Vector3::UnitZ);
    Game* game = owner->getGame();
    game->getRenderer()->setMirrorView(view);
}

Vector3 MirrorCamera::computeCameraPos() const {
    // Set camera position behind and above owner
    Vector3 cameraPos = owner->getPosition();
    cameraPos += owner->getForward() * horzDist;
    cameraPos += Vector3::UnitZ * vertDist;

    return cameraPos;
}

void MirrorCamera::setHorzDist(float dist) {
    horzDist = dist;
}

void MirrorCamera::setVertDist(float dist) {
    vertDist = dist;
}

void MirrorCamera::setTargetDist(float dist) {
    targetDist = dist;
}

Component::TypeID MirrorCamera::getType() const {
    return Component::TypeID::MirrorCamera;
}

void MirrorCamera::loadProperties(const rapidjson::Value& inObject) {
    CameraComponent::loadProperties(inObject);

    JsonHelper::getFloat(inObject, "horzDist", horzDist);
    JsonHelper::getFloat(inObject, "vertDist", vertDist);
    JsonHelper::getFloat(inObject, "targetDist", targetDist);
}