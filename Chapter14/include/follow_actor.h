#ifndef FOLLOW_ACTOR_H
#define FOLLOW_ACTOR_H

#include "actor.h"

class FollowCamera;
class Game;
class SkeletalMeshComponent;
class MoveComponent;

class FollowActor : public Actor {
public:
    FollowActor(Game* game);

    void actorInput(const bool* keys) override;

    void setVisible(bool visible);

    void loadProperties(const rapidjson::Value& inObj) override;

private:
    bool moving;
    MoveComponent* moveComp;
    FollowCamera* cameraComp;
    SkeletalMeshComponent* meshComp;
};

#endif