#include "component.h"

#include "actor.h"

Component::Component(Actor* owner, int updateOrder) : owner(owner), updateOrder(updateOrder) {
    owner->addComponent(this);
}

Component::~Component() {
    owner->removeComponent(this);
}

void Component::processInput(const bool* keyState) {}

void Component::update(float deltaTime) {}

int Component::getUpdateOrder() const {
    return updateOrder;
}
