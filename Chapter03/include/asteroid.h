#ifndef ASTEROID_H
#define ASTEROID_H

#include "actor.h"

class Game;
class CircleComponent;

class Asteroid : public Actor {
public:
    Asteroid(Game* game);

    CircleComponent* getCircle() const;

private:
    CircleComponent* circle;
};

#endif