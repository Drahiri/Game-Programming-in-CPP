#ifndef CAMERA_ACTOR_H
#define CAMERA_ACTOR_H

#include "actor.h"
#include "sound_event.h"

class Game;
class MoveComponent;
class AudioComponent;

class CameraActor : public Actor {
public:
    CameraActor(Game* game);

    void updateActor(float deltaTime) override;
    void actorInput(const bool* keys) override;

    void setFootstepSurface(float value);

private:
    MoveComponent* moveComp;

    AudioComponent* audioComp;
    SoundEvent footstep;

    float lastFootstep;
};

#endif