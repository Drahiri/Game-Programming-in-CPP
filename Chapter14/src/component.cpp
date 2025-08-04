#include "component.h"

#include "actor.h"
#include "level_loader.h"

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