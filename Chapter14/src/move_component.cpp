#include "move_component.h"

#include "actor.h"
#include "game.h"
#include "renderer.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder) :
    Component(owner, updateOrder),
    angularSpeed(0.0f),
    forwardSpeed(0.0f),
    strafeSpeed(0.0f) {
    screenSize.x = owner->getGame()->getRenderer()->getScreenWidth();
    screenSize.y = owner->getGame()->getRenderer()->getScreenHeight();
}

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

    if(!Math::NearZero(forwardSpeed) || !Math::NearZero(strafeSpeed)) {
        Vector3 pos = owner->getPosition();
        pos += owner->getForward() * forwardSpeed * deltaTime;
        // Update position based on strafe
        pos += owner->getRight() * strafeSpeed * deltaTime;
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

float MoveComponent::getStrafeSpeed() const {
    return strafeSpeed;
}

void MoveComponent::setStrafeSpeed(float speed) {
    strafeSpeed = speed;
}

Component::TypeID MoveComponent::getType() const {
    return Component::TypeID::MoveComponent;
}