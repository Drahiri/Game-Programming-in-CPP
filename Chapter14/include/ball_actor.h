#ifndef BALL_ACTOR_H
#define BALL_ACTOR_H

#include "actor.h"

class AudioComponent;
class BallMove;
class Game;

class BallActor : public Actor {
public:
    BallActor(Game* game);

    void updateActor(float deltaTime) override;
    void setPlayer(Actor* player);

    void hitTarget();

    void loadProperties(const rapidjson::Value& inObj) override;

private:
    AudioComponent* audioComp;
    BallMove* myMove;
    float lifeSpan;
};

#endif