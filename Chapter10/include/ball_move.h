#ifndef BALL_MOVE_H
#define BALL_MOVE_H

#include "move_component.h"

class Actor;

class BallMove : public MoveComponent {
public:
    BallMove(Actor* owner);

    void update(float deltaTime) override;
};
#endif