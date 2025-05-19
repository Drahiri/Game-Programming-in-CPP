#ifndef ENEMY_H
#define ENEMY_H

#include "actor.h"

class Game;
class CircleComponent;

class Enemy : public Actor {
public:
    Enemy(Game* game);
    ~Enemy();
    void updateActor(float deltaTime) override;
    CircleComponent* getCircle();

private:
    CircleComponent* circle;
};

#endif