#ifndef BULLET_H
#define BULLET_H

#include "actor.h"

class Game;
class CircleComponent;

class Bullet : public Actor {
public:
    Bullet(Game* game);
    void updateActor(float deltaTime) override;

private:
    CircleComponent* circle;
    float liveTime;
};

#endif