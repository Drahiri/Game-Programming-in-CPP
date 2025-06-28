#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "math.h"

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

private:
    // Store mouse position
    Vector2 mousePos;
    // Store button data
    Uint32 currButtons;
    Uint32 prevButtons;
    // Is relative
    bool isRelative;
};

// Wrapper that contains current state of input
struct InputState {
    KeyboardState keyboard;
    MouseState mouse;
};

class InputSystem {
public:
    bool initialize();
    void shutdown();

    // Called right before SDL_PollEvents loop
    void prepareForUpdate();
    // Called right after SDL_PollEvents loop
    void update();

    const InputState& getState() const;

    void setRelativeMouseMode(SDL_Window* window, bool value);

private:
    InputState state;
};

#endif