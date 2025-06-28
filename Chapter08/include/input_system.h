#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

enum class ButtonState {
    None,
    Pressed,
    Released,
    Held
};

// Wrapper that contains current state of input
struct InputState {};

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