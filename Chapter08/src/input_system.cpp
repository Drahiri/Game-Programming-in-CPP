#include "input_system.h"

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

const Vector2& MouseState::getScrollWheel() const {
    return scrollWheel;
}

bool InputSystem::initialize() {
    // Keyboard
    // Assign current state pointer
    state.keyboard.currState = SDL_GetKeyboardState(NULL);
    // Clear previous state memory
    memset(state.keyboard.prevState, 0, SDL_SCANCODE_COUNT);

    // Mouse
    state.mouse.isRelative = false;

    // Gamepad
    state.gamepad = SDL_OpenGamepad(0);
    return true;
}

void InputSystem::shutdown() {}

void InputSystem::prepareForUpdate() {
    // Keyboard
    memcpy(state.keyboard.prevState, state.keyboard.currState, SDL_SCANCODE_COUNT);

    // Mouse
    state.mouse.prevButtons = state.mouse.currButtons;
    state.mouse.scrollWheel = Vector2::Zero;
}

void InputSystem::update() {
    // Mouse
    float x = 0.0f, y = 0.0f;
    if(state.mouse.isRelative) {
        state.mouse.currButtons = SDL_GetRelativeMouseState(&x, &y);
    } else {
        state.mouse.currButtons = SDL_GetMouseState(&x, &y);
    }

    state.mouse.mousePos.x = x;
    state.mouse.mousePos.y = y;
}

void InputSystem::processEvent(SDL_Event& event) {
    switch(event.type) {
    case SDL_EVENT_MOUSE_WHEEL:
        state.mouse.scrollWheel = Vector2(event.wheel.x, event.wheel.y);
        break;

    default:
        break;
    }
}

const InputState& InputSystem::getState() const {
    return state;
}

void InputSystem::setRelativeMouseMode(SDL_Window* window, bool value) {
    SDL_SetWindowRelativeMouseMode(window, value);
    state.mouse.isRelative = value;
}
