#include "move_component.h"

#include "actor.h"
#include "game.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder) :
    Component(owner, updateOrder),
    angularSpeed(0.0f),
    forwardSpeed(0.0f),
    screenSize(owner->getGame()->getScreenSize()) {}

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

        // If out of screen move to other end
        if(pos.x <= -screenSize.x / 2) {
            pos.x = screenSize.x / 2;
        } else if(pos.x >= screenSize.x / 2) {
            pos.x = -screenSize.x / 2;
        }
        if(pos.y <= -screenSize.y) {
            pos.y = screenSize.y;
        } else if(pos.y >= screenSize.y) {
            pos.y = -screenSize.y;
        }

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
