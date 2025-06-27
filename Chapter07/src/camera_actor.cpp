#include "camera_actor.h"

#include "audio_component.h"
#include "audio_system.h"
#include "game.h"
#include "math.h"
#include "mesh_component.h"
#include "move_component.h"
#include "renderer.h"

CameraActor::CameraActor(Game* game) : Actor(game) {
    moveComp = new MoveComponent(this);
    audioComp = new AudioComponent(this);

    MeshComponent* mc = new MeshComponent(this);
    mc->setMesh(game->getRenderer()->getMesh("assets/Sphere.gpmesh"));

    lastFootstep = 0.0f;
    footstep = audioComp->playEvent("event:/Footstep");
    footstep.setPaused(true);
}

void CameraActor::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    // Play the footstep if we're moving and havent't recently
    lastFootstep -= deltaTime;
    if(!Math::NearZero(getVelocity().Length()) && lastFootstep <= 0.0f) {
        footstep.setPaused(false);
        footstep.restart();
        lastFootstep = 0.5f;
    }

    // Compute new camera from this actor
    cameraPos = getPosition() - getForward() * 200.0f + Vector3::UnitZ * 100.0f;
    Vector3 target = getPosition() + getForward() * 100.0f;
    Vector3 up = Vector3::UnitZ;
    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    getGame()->getRenderer()->setViewMatrix(view);
    getGame()->getAudioSystem()->setListener(view, getVelocity());
}

void CameraActor::actorInput(const bool* keys) {
    float forwardSpeed = 0.0f;
    float angularSpeed = 0.0f;
    // wasd movement
    if(keys[SDL_SCANCODE_W]) {
        // forwardSpeed += 300.0f;
        moveComp->addForce(getForward() * 300.0f);
    }
    if(keys[SDL_SCANCODE_S]) {
        // forwardSpeed -= 300.0f;
        moveComp->addForce(getForward() * -300.0f);
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
    footstep.setPaused(true);
    footstep.setParameter("Surface", value);
}

const Vector3& CameraActor::getCameraPosition() const {
    return cameraPos;
}