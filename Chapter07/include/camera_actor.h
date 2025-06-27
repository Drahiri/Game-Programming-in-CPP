#ifndef CAMERA_ACTOR_H
#define CAMERA_ACTOR_H

#include "actor.h"
#include "math.h"
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

    const Vector3& getCameraPosition() const;

private:
    MoveComponent* moveComp;
    Vector3 cameraPos;

    AudioComponent* audioComp;
    SoundEvent footstep;

    float lastFootstep;
};

#endif