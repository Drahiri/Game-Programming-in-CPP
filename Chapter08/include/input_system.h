#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <SDL3/SDL_scancode.h>

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

// Wrapper that contains current state of input
struct InputState {
    KeyboardState keyboard;
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

private:
    InputState state;
};

#endif