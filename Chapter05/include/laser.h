#ifndef LASER_H
#define LASER_H

#include "actor.h"

class Game;
class CircleComponent;

class Laser : public Actor {
public:
    Laser(Game* game);

    void updateActor(float deltaTime) override;

private:
    CircleComponent* circle;
    float deathTimer;
};

#endif