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

bool GamepadState::getButtonValue(SDL_GamepadButton button) const {
    return currButtons[button];
}

ButtonState GamepadState::getButtonState(SDL_GamepadButton button) const {
    if(prevButtons[button] == 0) {
        if(currButtons[button] == 0) {
            return ButtonState::None;
        } else {
            return ButtonState::Pressed;
        }
    } else {
        if(currButtons[button] == 0) {
            return ButtonState::Released;
        } else {
            return ButtonState::Held;
        }
    }
}

bool GamepadState::getIsConnected() const {
    return isConnected;
}

float GamepadState::getLeftTrigger() const {
    return leftTrigger;
}

float GamepadState::getRightTrigger() const {
    return rightTrigger;
}

const Vector2& GamepadState::getLeftStick() const {
    return leftStick;
}

const Vector2& GamepadState::getRightStick() const {
    return rightStick;
}

bool InputSystem::initialize() {
    // Keyboard
    // Assign current state pointer
    state.keyboard.currState = SDL_GetKeyboardState(NULL);
    // Clear previous state memory
    memset(state.keyboard.prevState, 0, SDL_SCANCODE_COUNT);

    // Mouse
    state.mouse.isRelative = false;

    return true;
}

void InputSystem::shutdown() {
    for(int i = 0; i < gamepads.size(); i++) {
        if(gamepads[i] != nullptr) {
            SDL_CloseGamepad(gamepads[i]);
        }
    }
}

void InputSystem::prepareForUpdate() {
    // Keyboard
    memcpy(state.keyboard.prevState, state.keyboard.currState, SDL_SCANCODE_COUNT);

    // Mouse
    state.mouse.prevButtons = state.mouse.currButtons;
    state.mouse.scrollWheel = Vector2::Zero;

    // Gamepad
    for(int i = 0; i < gamepads.size(); i++) {
        if(gamepads[i] != nullptr) {
            memcpy(state.gamepads[i].prevButtons,
                  state.gamepads[i].currButtons,
                  SDL_GAMEPAD_BUTTON_COUNT);
        }
    }
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

    // Gamepad
    for(int j = 0; j < gamepads.size(); j++) {
        if(gamepads[j] == nullptr) {
            continue;
        }

        for(int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++) {
            state.gamepads[j].currButtons[i] =
                  SDL_GetGamepadButton(gamepads[j], SDL_GamepadButton(i));
        }

        state.gamepads[j].leftTrigger =
              filter1D(SDL_GetGamepadAxis(gamepads[j], SDL_GAMEPAD_AXIS_LEFT_TRIGGER));
        state.gamepads[j].rightTrigger =
              filter1D(SDL_GetGamepadAxis(gamepads[j], SDL_GAMEPAD_AXIS_RIGHT_TRIGGER));

        x = SDL_GetGamepadAxis(gamepads[j], SDL_GAMEPAD_AXIS_LEFTX);
        y = -SDL_GetGamepadAxis(gamepads[j], SDL_GAMEPAD_AXIS_LEFTY);
        state.gamepads[j].leftStick = filter2D(x, y);

        x = SDL_GetGamepadAxis(gamepads[j], SDL_GAMEPAD_AXIS_RIGHTX);
        y = -SDL_GetGamepadAxis(gamepads[j], SDL_GAMEPAD_AXIS_RIGHTY);
        state.gamepads[j].rightStick = filter2D(x, y);
    }
}

void InputSystem::processEvent(SDL_Event& event) {
    switch(event.type) {
    case SDL_EVENT_MOUSE_WHEEL:
        state.mouse.scrollWheel = Vector2(event.wheel.x, event.wheel.y);
        break;

    case SDL_EVENT_GAMEPAD_ADDED:
        addGamepad(event.gdevice.which);
        break;

    case SDL_EVENT_GAMEPAD_REMOVED:
        removeGamepad(event.gdevice.which);
        break;

    default:
        break;
    }
}

void InputSystem::addGamepad(SDL_JoystickID id) {
    // Find if there's slot
    int i = 0;
    for(; i < gamepads.size(); i++) {
        if(gamepads[i] == nullptr) {
            break;
        }
    }
    if(i == gamepads.size()) {
        SDL_Log("Can't add gamepad. Too many");
        return;
    }

    gamepads[i] = SDL_OpenGamepad(id);
    state.gamepads[i].isConnected = (gamepads[i] != nullptr);
    memset(state.gamepads[i].currButtons, 0, SDL_GAMEPAD_BUTTON_COUNT);
    memset(state.gamepads[i].prevButtons, 0, SDL_GAMEPAD_BUTTON_COUNT);
}

void InputSystem::removeGamepad(SDL_JoystickID id) {
    for(int i = 0; i < gamepads.size(); i++) {
        if(gamepads[i] == nullptr) {
            continue;
        }

        if(SDL_GetGamepadID(gamepads[i]) == id) {
            SDL_CloseGamepad(gamepads[i]);
            gamepads[i] = nullptr;
            break;
        }
    }
}

const InputState& InputSystem::getState() const {
    return state;
}

void InputSystem::setRelativeMouseMode(SDL_Window* window, bool value) {
    SDL_SetWindowRelativeMouseMode(window, value);
    state.mouse.isRelative = value;
}

float InputSystem::filter1D(int input) {
    // A value < dead zone is interpreted as 0%
    const int deadZone = 250;
    // A value > max value is interpreted as 100%
    const int maxValue = 30000;

    float retVal = 0.0f;

    // Take absolute value of input
    int absValue = input > 0 ? input : -input;
    // Ignore input within dead zone
    if(absValue > deadZone) {
        // Compute fractional value between dead zone and max value
        retVal = static_cast<float>(absValue - deadZone) / (maxValue - deadZone);

        // Make sure sign mastcher original value
        retVal = input > 0 ? retVal : -1.0f * retVal;

        // Clamp between -1.0f and 1.0f
        retVal = Math::Clamp(retVal, -1.0f, 1.0f);
    }

    return retVal;
}

Vector2 InputSystem::filter2D(int inputX, int inputY) {
    const float deadZone = 8000.0f;
    const float maxValue = 30000.0f;

    // Make into 2D vector
    Vector2 dir;
    dir.x = static_cast<float>(inputX);
    dir.y = static_cast<float>(inputY);

    float length = dir.Length();

    // If length < deadZone, should be no input
    if(length < deadZone) {
        dir = Vector2::Zero;
    } else {
        // Calculate fractional value between
        // dead zone and max value circles
        float f = (length - deadZone) / (maxValue - deadZone);
        // Clamp f between 0.0f and 1.0f
        f = Math::Clamp(f, 0.0f, 1.0f);
        // Normalize the vector, and then scale it to the fractional value
        dir *= f / length;
    }

    return dir;
}