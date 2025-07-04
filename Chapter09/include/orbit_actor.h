#ifndef ORBIT_ACTOR_H
#define ORBIT_ACTOR_H

#include "actor.h"
#include "sound_event.h"

class Game;
class MeshComponent;
class OrbitCamera;

class OrbitActor : public Actor {
public:
    OrbitActor(Game* game);

    void actorInput(const bool* keyState) override;

    void setVisible(bool visible);

private:
    MeshComponent* meshComp;
    OrbitCamera* cameraComp;
};

#endif