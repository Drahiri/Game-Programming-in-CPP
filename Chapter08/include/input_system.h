#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "math.h"

#include <array>
#include <SDL3/SDL.h>

// The different button states
enum class ButtonState {
    None,
    Pressed,
    Released,
    Held
};

// Helper class for keyboard input
class KeyboardState {
public:
    // Frient do InputSystem can easily update it
    friend class InputSystem;

    // Get just the boolean true/false value of key
    bool getKeyValue(SDL_Scancode keyCode) const;

    // Get a state based on current and previous frame
    ButtonState getKeyState(SDL_Scancode keyCode) const;

private:
    // Current state
    const bool* currState;
    bool prevState[SDL_SCANCODE_COUNT];
};

// Helper class for mouse input
class MouseState {
public:
    friend class InputSystem;
    // For mouse position
    const Vector2& getPosition() const;

    // For buttons
    bool getButtonValue(int button) const;
    ButtonState getButtonState(int button) const;
    // For scroll wheel
    const Vector2& getScrollWheel() const;

private:
    // Store mouse position
    Vector2 mousePos;
    // Store button data
    Uint32 currButtons;
    Uint32 prevButtons;
    // Is relative
    bool isRelative;
    // Scroll wheel
    Vector2 scrollWheel;
};

// Helper class for gamepads
class GamepadState {
public:
    friend class InputSystem;

    // For buttons
    bool getButtonValue(SDL_GamepadButton button) const;
    ButtonState getButtonState(SDL_GamepadButton button) const;

    // For triggers
    float getLeftTrigger() const;
    float getRightTrigger() const;

    // For sticks
    const Vector2& getLeftStick() const;
    const Vector2& getRightStick() const;

    bool getIsConnected() const;

private:
    // Current/previous buttons
    Uint8 currButtons[SDL_GAMEPAD_BUTTON_COUNT];
    Uint8 prevButtons[SDL_GAMEPAD_BUTTON_COUNT];

    // Is this gamepad connected?
    bool isConnected;

    // Triggers
    float leftTrigger;
    float rightTrigger;

    // Sticks
    Vector2 leftStick;
    Vector2 rightStick;
};

// Wrapper that contains current state of input
struct InputState {
    KeyboardState keyboard;
    MouseState mouse;
    std::array<GamepadState, 4> gamepads;
};

class InputSystem {
public:
    bool initialize();
    void shutdown();

    // Called right before SDL_PollEvents loop
    void prepareForUpdate();
    // Called right after SDL_PollEvents loop
    void update();
    // Mouse wheel
    void processEvent(SDL_Event& event);

    const InputState& getState() const;

    void setRelativeMouseMode(SDL_Window* window, bool value);

    float filter1D(int input);
    Vector2 filter2D(int inputX, int inputY);

private:
    InputState state;
    std::array<SDL_Gamepad*, 4> gamepads;
};

#endif