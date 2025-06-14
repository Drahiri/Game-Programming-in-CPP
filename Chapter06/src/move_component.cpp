#include "move_component.h"

#include "actor.h"
#include "game.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder) :
    Component(owner, updateOrder),
    angularSpeed(0.0f),
    forwardSpeed(0.0f) {}

void MoveComponent::update(float deltaTime) {
    if(!Math::NearZero(angularSpeed)) {
        Quaternion rot = owner->getRotation();
        float angle = angularSpeed * deltaTime;

        // Create quaternion for incremental rotation
        // (Rotate about up axis)
        Quaternion inc(Vector3::UnitZ, angle);

        // Concatenate old and new quaternion
        rot = Quaternion::Concatenate(rot, inc);
        owner->setRotation(rot);
    }

    if(!Math::NearZero(forwardSpeed)) {
        Vector3 pos = owner->getPosition();
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
