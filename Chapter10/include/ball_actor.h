#ifndef BALL_ACTOR_H
#define BALL_ACTOR_H

#include "actor.h"

class BallMove;
class Game;

class BallActor : public Actor {
public:
    BallActor(Game* game);

    void updateActor(float deltaTime) override;

private:
    BallMove* myMove;
};

#endif