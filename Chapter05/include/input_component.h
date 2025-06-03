#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include "move_component.h"

class Actor;

class InputComponent : public MoveComponent {
public:
    InputComponent(Actor* owner);

    void processInput(const bool* keyState) override;

    // Getters/setters for private variables
    float getMaxForwardSpeed() const;
    void setMaxForwardSpeed(float speed);

    float getMaxAngularSpeed() const;
    void setMaxAngularSpeed(float speed);

    int getForwardKey() const;
    void setForwardKey(int key);
    int getBackKey() const;
    void setBackKey(int key);

    int getClockwiseKey() const;
    void setClockwiseKey(int key);
    int getCounterClockwiseKey() const;
    void setCounterClockwiseKey(int key);

private:
    // The maximum forward/angular speeds
    float maxForwardSpeed;
    float maxAngularSpeed;

    // Keys for forward/back movement
    int forwardKey;
    int backKey;

    // Keys for angular movement
    int clockwiseKey;
    int counterClockwiseKey;
};

#endif