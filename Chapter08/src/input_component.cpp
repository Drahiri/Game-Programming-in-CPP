#include "input_component.h"

#include "input_system.h"

InputComponent::InputComponent(Actor* owner) : MoveComponent(owner) {}

void InputComponent::processInput(const InputState& inState) {
    // Calculate forward speed for MoveComponent
    float forwardSpeed = 0.0f;
    if(inState.keyboard.getKeyValue(forwardKey)) {
        forwardSpeed += maxForwardSpeed;
    }
    if(inState.keyboard.getKeyValue(backKey)) {
        forwardSpeed -= maxForwardSpeed;
    }

    setForwardSpeed(forwardSpeed);

    // Calculate angular speed for MoveComponent
    float angularSpeed = 0.0f;
    if(inState.keyboard.getKeyValue(clockwiseKey)) {
        angularSpeed += maxAngularSpeed;
    }
    if(inState.keyboard.getKeyValue(counterClockwiseKey)) {
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

void InputComponent::setForwardKey(SDL_Scancode key) {
    forwardKey = key;
}

int InputComponent::getBackKey() const {
    return backKey;
}

void InputComponent::setBackKey(SDL_Scancode key) {
    backKey = key;
}

int InputComponent::getClockwiseKey() const {
    return clockwiseKey;
}

void InputComponent::setClockwiseKey(SDL_Scancode key) {
    clockwiseKey = key;
}

int InputComponent::getCounterClockwiseKey() const {
    return counterClockwiseKey;
}

void InputComponent::setCounterClockwiseKey(SDL_Scancode key) {
    counterClockwiseKey = key;
}
