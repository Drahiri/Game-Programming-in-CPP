#include "box_component.h"

#include "actor.h"
#include "game.h"
#include "level_loader.h"
#include "phys_world.h"

BoxComponent::BoxComponent(Actor* owner) :
    Component(owner),
    objectBox(Vector3::Zero, Vector3::Zero),
    worldBox(Vector3::Zero, Vector3::Zero),
    shouldRotate(true) {
    owner->getGame()->getPhysWorld()->addBox(this);
}

BoxComponent::~BoxComponent() {
    owner->getGame()->getPhysWorld()->removeBox(this);
}

void BoxComponent::onUpdateWorldTransform() {
    // Reset to object space box
    worldBox = objectBox;
    // Scale
    worldBox.min *= owner->getScale();
    worldBox.max *= owner->getScale();
    // Roteta (if we want to)
    if(shouldRotate) {
        worldBox.rotate(owner->getRotation());
    }
    // Translate
    worldBox.min += owner->getPosition();
    worldBox.max += owner->getPosition();
}

void BoxComponent::setObjectBox(const AABB& model) {
    objectBox = model;
}

const AABB& BoxComponent::getWorldBox() const {
    return worldBox;
}

void BoxComponent::setShouldRotate(bool value) {
    shouldRotate = value;
}

Component::TypeID BoxComponent::getType() const {
    return Component::TypeID::BoxComponent;
}

void BoxComponent::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getVector3(inObj, "objectMin", objectBox.min);
    JsonHelper::getVector3(inObj, "objectMax", objectBox.max);
    JsonHelper::getVector3(inObj, "worldMin", worldBox.min);
    JsonHelper::getVector3(inObj, "worldMax", worldBox.max);
    JsonHelper::getBool(inObj, "shouldRotate", shouldRotate);
}

void BoxComponent::saveProperties(
      rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject) const {
    Component::saveProperties(alloc, inObject);

    JsonHelper::addVector3(alloc, inObject, "objectMin", objectBox.min);
    JsonHelper::addVector3(alloc, inObject, "objectMax", objectBox.max);
    JsonHelper::addVector3(alloc, inObject, "worldMin", worldBox.min);
    JsonHelper::addVector3(alloc, inObject, "worldMax", worldBox.max);
    JsonHelper::addBool(alloc, inObject, "shouldRotate", shouldRotate);
}
