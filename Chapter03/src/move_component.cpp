#include "move_component.h"

#include "actor.h"
#include "math.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder) :
    Component(owner, updateOrder),
    angularSpeed(0.0f),
    forwardSpeed(0.0f) {}

void MoveComponent::update(float deltaTime) {
    if(!Math::NearZero(angularSpeed)) {
        float rot = owner->getRotation();
        rot += angularSpeed * deltaTime;
        owner->setRotation(rot);
    }

    if(!Math::NearZero(forwardSpeed)) {
        Vector2 pos = owner->getPosition();
        pos += owner->getForward() * forwardSpeed * deltaTime;
        owner->setPosition(pos);
    }
}

float MoveComponent::getAngularSpeed() const {
    return angularSpeed;
}

void MoveComponent::setAngularSpeed(float speed) {
    angularSpeed = speed;
}

float MoveComponent::getForwardSpeed() const {
    return forwardSpeed;
}

void MoveComponent::setForwardSpeed(float speed) {
    forwardSpeed = speed;
}
