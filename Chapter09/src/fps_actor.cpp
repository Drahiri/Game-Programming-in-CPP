#include "fps_actor.h"

#include "audio_component.h"
#include "game.h"
#include "mesh_component.h"
#include "move_component.h"

FPSActor::FPSActor(Game* game) : Actor(game) {
    moveComp = new MoveComponent(this);
    audioComp = new AudioComponent(this);
    lastFootstep = 0.0f;
    footstep = audioComp->playEvent("event:/Footstep");
    footstep.setPaused(true);
}

void FPSActor::actorInput(const bool* keyState) {
    float forwardSpeed = 0.0f;
    float strafeSpeed = 0.0f;
    // wasd movement
    if(keyState[SDL_SCANCODE_W]) {
        forwardSpeed += 400.0f;
    }
    if(keyState[SDL_SCANCODE_S]) {
        forwardSpeed -= 400.0f;
    }
    if(keyState[SDL_SCANCODE_A]) {
        strafeSpeed -= 400.0f;
    }
    if(keyState[SDL_SCANCODE_D]) {
        strafeSpeed += 400.0f;
    }

    moveComp->setForwardSpeed(forwardSpeed);
    moveComp->setStrafeSpeed(strafeSpeed);
}
