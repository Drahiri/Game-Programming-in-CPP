#include "fps_actor.h"

#include "audio_component.h"
#include "audio_system.h"
#include "fps_camera.h"
#include "game.h"
#include "mesh_component.h"
#include "move_component.h"
#include "renderer.h"

FPSActor::FPSActor(Game* game) : Actor(game) {
    moveComp = new MoveComponent(this);
    audioComp = new AudioComponent(this);
    cameraComp = new FPSCamera(this);

    lastFootstep = 0.0f;
    footstep = audioComp->playEvent("event:/Footstep");
    footstep.setPaused(true);

    fpsModel = new Actor(game);
    fpsModel->setScale(0.75f);
    meshComp = new MeshComponent(fpsModel);
    meshComp->setMesh(game->getRenderer()->getMesh("assets/Rifle.gpmesh"));
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

    // Mouse
    float x, y;
    Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);

    // Assume mouse movement is usually between -500 and 500
    const int maxMouseSpeed = 500;
    // Rotation/sec at maximum speed
    const float maxAngularSpeed = Math::Pi * 8;
    float angularSpeed = 0.0f;

    if(x != 0) {
        // Convert to approximately [-1.0, 1.0]
        angularSpeed = x / maxMouseSpeed;
        // Multiply by rotation/sec
        angularSpeed *= maxAngularSpeed;
    }

    moveComp->setAngularSpeed(angularSpeed);

    // Compute pitch
    const float maxPitchSpeed = Math::Pi * 8;
    float pitchSpeed = 0.0f;
    if(y != 0) {
        // Convert to [-1.0, 1.0]
        pitchSpeed = y / maxPitchSpeed;
        pitchSpeed *= maxPitchSpeed;
    }

    cameraComp->setPitchSpeed(pitchSpeed);
}

void FPSActor::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    // Play the footstep if we're moving and havent't recently
    lastFootstep -= deltaTime;
    if(!Math::NearZero(moveComp->getForwardSpeed()) && lastFootstep <= 0.0f) {
        footstep.setPaused(false);
        footstep.restart();
        lastFootstep = 0.5f;
    }

    // Update position of FPS model relative to actor position
    const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
    Vector3 modelPos = getPosition();
    modelPos += getForward() * modelOffset.x;
    modelPos += getRight() * modelOffset.y;
    modelPos.z += modelOffset.z;
    fpsModel->setPosition(modelPos);

    // Initialize rotation to actor rotation
    Quaternion q = getRotation();

    // Rotate by pitch from camera
    q = Quaternion::Concatenate(q, Quaternion(getRight(), cameraComp->getPitch()));
    fpsModel->setRotation(q);
}

void FPSActor::setFootstepSurface(float value) {
    // Pause here because the way parameter is setup for FMOD
    // changing it will play a footstep
    footstep.setPaused(true);
    footstep.setParameter("Surface", value);
}