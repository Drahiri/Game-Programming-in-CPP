#ifndef TOWER_H
#define TOWER_H

#include "actor.h"

class Game;
class MoveComponent;

class Tower : public Actor {
public:
    Tower(Game* game);
    void updateActor(float deltaTime) override;

private:
    MoveComponent* move;
    float nextAttack;
    const float attackTime = 2.5f;
    const float attackRange = 100.0f;
};

#endif