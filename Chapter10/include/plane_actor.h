#ifndef PLANE_ACTOR_H
#define PLANE_ACTOR_H

#include "actor.h"

class Game;
class BoxComponent;

class PlaneActor : public Actor {
public:
    PlaneActor(Game* game);
    ~PlaneActor();
    BoxComponent* getBox();

private:
    BoxComponent* box;
};

#endif