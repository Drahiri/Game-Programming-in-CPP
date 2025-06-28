#include "input_system.h"

bool InputSystem::initialize() {
    return false;
}

void InputSystem::shutdown() {}

void InputSystem::prepareForUpdate() {}

void InputSystem::update() {}

const InputState& InputSystem::getState() const {
    return state;
}