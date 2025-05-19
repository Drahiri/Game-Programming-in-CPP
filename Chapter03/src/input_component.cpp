#include "input_component.h"

#include "actor.h"

InputComponent::InputComponent(Actor* owner) : MoveComponent(owner) {}

void InputComponent::processInput(const bool* keyState) {
    // Calculate forward speed for MoveComponent
    if(keyState[forwardKey]) {
        addForce(owner->getForward() * maxForwardSpeed);
    }
    if(keyState[backKey]) {
        addForce(owner->getForward() * -maxForwardSpeed);
    }

    // Calculate angular speed for MoveComponent
    float angularSpeed = 0.0f;
    if(keyState[clockwiseKey]) {
        angularSpeed += maxAngularSpeed;
    }
    if(keyState[counterClockwiseKey]) {
        angularSpeed -= maxAngularSpeed;
    }
    setAngularSpeed(angularSpeed);
}

float InputComponent::getMaxForwardSpeed() const {
    return maxForwardSpeed;
}

void InputComponent::setMaxForwardSpeed(float speed) {
    maxForwardSpeed = speed;
}

float InputComponent::getMaxAngularSpeed() const {
    return maxAngularSpeed;
}

void InputComponent::setMaxAngularSpeed(float speed) {
    maxAngularSpeed = speed;
}

int InputComponent::getForwardKey() const {
    return forwardKey;
}

void InputComponent::setForwardKey(int key) {
    forwardKey = key;
}

int InputComponent::getBackKey() const {
    return backKey;
}

void InputComponent::setBackKey(int key) {
    backKey = key;
}

int InputComponent::getClockwiseKey() const {
    return clockwiseKey;
}

void InputComponent::setClockwiseKey(int key) {
    clockwiseKey = key;
}

int InputComponent::getCounterClockwiseKey() const {
    return counterClockwiseKey;
}

void InputComponent::setCounterClockwiseKey(int key) {
    counterClockwiseKey = key;
}
