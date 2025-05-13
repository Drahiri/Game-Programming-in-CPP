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
        float rot = owner->getRotation();
        rot += angularSpeed * deltaTime;
        owner->setRotation(rot);
    }

    if(!Math::NearZero(forwardSpeed)) {
        Vector2 pos = owner->getPosition();
        pos += owner->getForward() * forwardSpeed * deltaTime;

        // If out of screen move to other end
        if(pos.x <= 0.0f) {
            pos.x = screenSize.x;
        } else if(pos.x >= screenSize.x) {
            pos.x = 0.0f;
        }
        if(pos.y <= 0.0f) {
            pos.y = screenSize.y;
        } else if(pos.y >= screenSize.y) {
            pos.y = 0.0f;
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
