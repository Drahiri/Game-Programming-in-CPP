#ifndef SPLINE_ACTOR_H
#define SPLINE_ACTOR_H

#include "actor.h"

class Game;
class SplineCamera;

class SplineActor : public Actor {
public:
    SplineActor(Game* game);

    void actorInput(const bool* keys) override;

    void restartSpline();

private:
    SplineCamera* cameraComp;
};

#endif
