#include "move_component.h"

#include "actor.h"
#include "game.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder) :
    Component(owner, updateOrder),
    angularSpeed(0.0f),
    forwardSpeed(0.0f),
    wrapable(false),
    screenSize(owner->getGame()->getScreenSize()),
    mass(1.0f),
    sumOfForces(Vector2::Zero),
    velocity(Vector2::Zero) {}

void MoveComponent::update(float deltaTime) {
    if(!Math::NearZero(angularSpeed)) {
        float rot = owner->getRotation();
        rot += angularSpeed * deltaTime;
        owner->setRotation(rot);
    }

    // The time step is already restricted by minimum FPS
    Vector2 pos = owner->getPosition();
    Vector2 accel = sumOfForces * (1 / mass); // Math.h does not have vector division
    velocity = accel * deltaTime;
    pos += velocity;

    // Clear sumOfForces
    sumOfForces = Vector2::Zero;

    // If out of screen move to other end
    if(wrapable) {
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
    }
    owner->setPosition(pos);
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

bool MoveComponent::getWrapable() const {
    return wrapable;
}

void MoveComponent::setWrapable(bool shouldWrap) {
    wrapable = shouldWrap;
}

float MoveComponent::getMass() const {
    return mass;
}

void MoveComponent::setMass(float newMass) {
    mass = newMass;
}

void MoveComponent::addForce(Vector2 force) {
    sumOfForces += force;
}
