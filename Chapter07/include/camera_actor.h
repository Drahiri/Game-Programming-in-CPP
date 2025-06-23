#ifndef CAMERA_ACTOR_H
#define CAMERA_ACTOR_H

#include "actor.h"

class Game;
class MoveComponent;

class CameraActor : public Actor {
public:
    CameraActor(Game* game);

    void updateActor(float deltaTime) override;
    void actorInput(const bool* keys) override;

    void setFootstepSurface(float value);

private:
    MoveComponent* moveComp;

    // TODO: uncomment after implementing sound
    /*
        AudioComponent* audioComp;
        SoundEvent footstep;
    */

    float lastFootstep;
};

#endif