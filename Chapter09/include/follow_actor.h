#ifndef FOLLOW_ACTOR_H
#define FOLLOW_ACTOR_H

#include "actor.h"

class FollowCamera;
class Game;
class MeshComponent;
class MoveComponent;

class FollowActor : public Actor {
public:
    FollowActor(Game* game);

    void actorInput(const bool* keys) override;

    void setVisible(bool visible);

private:
    MoveComponent* moveComp;
    FollowCamera* cameraComp;
    MeshComponent* meshComp;
};

#endif