#ifndef FPS_ACTOR_H
#define FPS_ACTOR_H

#include "actor.h"
#include "sound_event.h"

class Game;
class AudioComponent;
class MeshComponent;
class MoveComponent;

class FPSActor : public Actor {
public:
    FPSActor(Game* game);

    void actorInput(const bool* keyState) override;
    void updateActor(float deltaTime) override;

    void setFootstepSurface(float value);

private:
    MoveComponent* moveComp;
    AudioComponent* audioComp;
    MeshComponent* meshComp;

    SoundEvent footstep;
    float lastFootstep;
};

#endif