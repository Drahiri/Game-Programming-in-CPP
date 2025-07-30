#ifndef TARGET_ACTOR_H
#define TARGET_ACTOR_H

#include "actor.h"

class Game;

class TargetActor : public Actor {
public:
    TargetActor(Game* game);
};

#endif