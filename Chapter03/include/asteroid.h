#ifndef ASTEROID_H
#define ASTEROID_H

#include "actor.h"

class Game;
class CircleComponent;
class MoveComponent;

class Asteroid : public Actor {
public:
    Asteroid(Game* game);
    ~Asteroid();

    void updateActor(float deltaTime) override;

    CircleComponent* getCircle() const;

private:
    CircleComponent* circle;
    MoveComponent* mc;
};

#endif