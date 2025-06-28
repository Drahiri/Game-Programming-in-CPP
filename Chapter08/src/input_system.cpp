#include "input_system.h"

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

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

const Vector2& MouseState::getPosition() const {
    return mousePos;
}

bool MouseState::getButtonValue(int button) const {
    return (currButtons & SDL_BUTTON_MASK(button));
}

ButtonState MouseState::getButtonState(int button) const {
    if((prevButtons & SDL_BUTTON_MASK(button)) == 0) {
        if((currButtons & SDL_BUTTON_MASK(button)) == 0) {
            return ButtonState::None;
        } else {
            return ButtonState::Pressed;
        }
    } else {
        if((currButtons & SDL_BUTTON_MASK(button)) == 0) {
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

    state.mouse.prevButtons = state.mouse.currButtons;
}

void InputSystem::update() {
    // Mouse
    float x = 0.0f, y = 0.0f;
    state.mouse.currButtons = SDL_GetMouseState(&x, &y);
    state.mouse.mousePos.x = x;
    state.mouse.mousePos.y = y;
}

const InputState& InputSystem::getState() const {
    return state;
}
