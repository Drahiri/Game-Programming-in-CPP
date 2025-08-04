#ifndef BALL_MOVE_H
#define BALL_MOVE_H

#include "move_component.h"

class Actor;

class BallMove : public MoveComponent {
public:
    BallMove(Actor* owner);

    void setPlayer(Actor* player);
    void update(float deltaTime) override;

    Component::TypeID getType() const override;

private:
    Actor* player;
};
#endif