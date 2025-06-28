#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include "move_component.h"

#include <SDL3/SDL_scancode.h>

class Actor;
struct InputState;

class InputComponent : public MoveComponent {
public:
    InputComponent(Actor* owner);

    void processInput(const InputState& inState) override;

    // Getters/setters for private variables
    float getMaxForwardSpeed() const;
    void setMaxForwardSpeed(float speed);

    float getMaxAngularSpeed() const;
    void setMaxAngularSpeed(float speed);

    int getForwardKey() const;
    void setForwardKey(SDL_Scancode key);
    int getBackKey() const;
    void setBackKey(SDL_Scancode key);

    int getClockwiseKey() const;
    void setClockwiseKey(SDL_Scancode key);
    int getCounterClockwiseKey() const;
    void setCounterClockwiseKey(SDL_Scancode key);

private:
    // The maximum forward/angular speeds
    float maxForwardSpeed;
    float maxAngularSpeed;

    // Keys for forward/back movement
    SDL_Scancode forwardKey;
    SDL_Scancode backKey;

    // Keys for angular movement
    SDL_Scancode clockwiseKey;
    SDL_Scancode counterClockwiseKey;
};

#endif