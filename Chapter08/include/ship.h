#ifndef SHIP_H
#define SHIP_H

#include "actor.h"

class Game;
struct InputState;

class Ship : public Actor {
public:
    Ship(Game* game);

    void updateActor(float deltaTime) override;
    void actorInput(const InputState& inState) override;

    float getRightSpeed() const;
    float getDownSpeed() const;

private:
    float rightSpeed;
    float downSpeed;
    float laserCooldown;

    Vector2 velocitDir;
    Vector2 rotationDir;
    float speed;
};

#endif