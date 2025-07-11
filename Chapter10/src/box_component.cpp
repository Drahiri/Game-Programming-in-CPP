#include "box_component.h"

#include "actor.h"

BoxComponent::BoxComponent(Actor* owner) :
    Component(owner),
    objectBox(Vector3::Zero, Vector3::Zero),
    worldBox(Vector3::Zero, Vector3::Zero),
    shouldRotate(true) {}

BoxComponent::~BoxComponent() {}

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
