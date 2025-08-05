#include "component.h"

#include "actor.h"
#include "level_loader.h"

const char* Component::typeNames[static_cast<int>(Component::TypeID::NUM_COMPONENT_TYPES)]{
    "Component",
    "AudioComponent",
    "BallMove",
    "BoxComponent",
    "CameraComponent",
    "FollowCamera",
    "MeshComponent",
    "MoveComponent",
    "SkeletalMeshComponent",
    "SpriteComponent",
    "MirrorCamera",
    "PointLightComponent",
    "TargetComponent",
};

Component::Component(Actor* owner, int updateOrder) : owner(owner), updateOrder(updateOrder) {
    owner->addComponent(this);
}

Component::~Component() {
    owner->removeComponent(this);
}

void Component::processInput(const bool* keyState) {}

void Component::onUpdateWorldTransform() {}

void Component::update(float deltaTime) {}

int Component::getUpdateOrder() const {
    return updateOrder;
}

Actor* Component::getOwner() {
    return owner;
}

void Component::loadProperties(const rapidjson::Value& inObject) {
    JsonHelper::getInt(inObject, "updateOrder", updateOrder);
}

void Component::saveProperties(
      rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject) const {
    JsonHelper::addInt(alloc, inObject, "drawOrder", updateOrder);
}