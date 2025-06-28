#include "input_system.h"

#include <SDL3/SDL_keyboard.h>

bool KeyboardState::getKeyValue(SDL_Scancode keyCode) const {
    return currState[keyCode];
}

ButtonState KeyboardState::getKeyState(SDL_Scancode keyCode) const {
    if(prevState[keyCode] == 0) {
        if(currState[keyCode] == 0) {
            return ButtonState::None;
        } else {
            return ButtonState::Pressed;
        }
    } else // Prev state must be 1
    {
        if(currState[keyCode] == 0) {
            return ButtonState::Released;
        } else {
            return ButtonState::Held;
        }
    }
}

bool InputSystem::initialize() {
    // Assign current state pointer
    state.keyboard.currState = SDL_GetKeyboardState(NULL);

    // Clear previous state memory
    memset(state.keyboard.prevState, 0, SDL_SCANCODE_COUNT);
    return true;
}

void InputSystem::shutdown() {}

void InputSystem::prepareForUpdate() {
    memcpy(state.keyboard.prevState, state.keyboard.currState, SDL_SCANCODE_COUNT);
}

void InputSystem::update() {}

const InputState& InputSystem::getState() const {
    return state;
}
