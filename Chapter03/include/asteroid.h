#ifndef ASTEROID_H
#define ASTEROID_H

#include "actor.h"

class Game;

class Asteroid : public Actor {
public:
    Asteroid(Game* game);
};

#endif