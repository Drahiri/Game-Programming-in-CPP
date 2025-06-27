#ifndef DOPPLER_ACTOR_H
#define DOPPLER_ACTOR_H

#include <actor.h>

class Game;
class MoveComponent;

class DopplerActor : public Actor {
public:
    DopplerActor(Game* game);

    void updateActor(float deltaTime) override;

private:
    MoveComponent* mc;

    float time;
};

#endif