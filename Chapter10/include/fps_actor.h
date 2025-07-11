#ifndef FPS_ACTOR_H
#define FPS_ACTOR_H

#include "actor.h"
#include "sound_event.h"

class AudioComponent;
class BoxComponent;
class FPSCamera;
class Game;
class MeshComponent;
class MoveComponent;

class FPSActor : public Actor {
public:
    FPSActor(Game* game);

    void actorInput(const bool* keyState) override;
    void updateActor(float deltaTime) override;

    void setFootstepSurface(float value);

    void setVisible(bool visible);

private:
    MoveComponent* moveComp;
    AudioComponent* audioComp;
    MeshComponent* meshComp;
    FPSCamera* cameraComp;
    BoxComponent* boxComp;

    Actor* fpsModel;

    SoundEvent footstep;
    float lastFootstep;
};

#endif