#include "camera_actor.h"

#include "game.h"
#include "math.h"
#include "move_component.h"
#include "renderer.h"

CameraActor::CameraActor(Game* game) : Actor(game) {
    moveComp = new MoveComponent(this);
    lastFootstep = 0.0f;
}

void CameraActor::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    // Play the footstep if we're moving and havent't recently
    lastFootstep -= deltaTime;
    if(!Math::NearZero(moveComp->getForwardSpeed()) && lastFootstep <= 0.0f) {
        // TODO: Uncomment after implementing sound
        /*
        footstep.setPaused(false);
        footstep.restart();
        */
        lastFootstep = 0.5f;
    }

    // Compute new camera from this actor
    Vector3 cameraPos = getPosition();
    Vector3 target = getPosition() + getForward() * 100.0f;
    Vector3 up = Vector3::UnitZ;
    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    getGame()->getRenderer()->setViewMatrix(view);
    // TODO: Uncomment after implementing sound
    /*
    getGame()->getAudio()->setListener(view);
    */
}

void CameraActor::actorInput(const bool* keys) {
    float forwardSpeed = 0.0f;
    float angularSpeed = 0.0f;
    // wasd movement
    if(keys[SDL_SCANCODE_W]) {
        forwardSpeed += 300.0f;
    }
    if(keys[SDL_SCANCODE_S]) {
        forwardSpeed -= 300.0f;
    }
    if(keys[SDL_SCANCODE_A]) {
        angularSpeed -= Math::TwoPi;
    }
    if(keys[SDL_SCANCODE_D]) {
        angularSpeed += Math::TwoPi;
    }

    moveComp->setForwardSpeed(forwardSpeed);
    moveComp->setAngularSpeed(angularSpeed);
}

void CameraActor::setFootstepSurface(float value) {
    // Pause here because the way parameter is setup for FMOD
    // changing it will play a footstep
    // TODO: Uncomment after implementing sound
    /*
    footstep.setPaused(true);
    footstep.setParameter("Surface", value);
    */
}