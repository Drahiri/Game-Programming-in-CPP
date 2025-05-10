#ifndef SHIP_H
#define SHIP_H

#include "actor.h"

class Game;

class Ship : public Actor {
public:
    Ship(Game* game);

    void updateActor(float deltaTime) override;

    float getRightSpeed() const;
    float getDownSpeed() const;

private:
    float rightSpeed;
    float downSpeed;
};

#endif