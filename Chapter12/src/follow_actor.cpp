#include "follow_actor.h"

#include "follow_camera.h"
#include "game.h"
#include "move_component.h"
#include "renderer.h"
#include "skeletal_mesh_component.h"

FollowActor::FollowActor(Game* game) : Actor(game) {
    meshComp = new SkeletalMeshComponent(this);
    meshComp->setMesh(game->getRenderer()->getMesh("assets/CatWarrior.gpmesh"));
    setPosition(Vector3(0.0f, 0.0f, -100.0f));

    moveComp = new MoveComponent(this);
    cameraComp = new FollowCamera(this);
    cameraComp->snapToIdeal();
}

void FollowActor::actorInput(const bool* keys) {
    float forwardSpeed = 0.0f;
    float angularSpeed = 0.0f;
    if(keys[SDL_SCANCODE_W]) {
        forwardSpeed += 400.0f;
    }
    if(keys[SDL_SCANCODE_S]) {
        forwardSpeed -= 400.0f;
    }
    if(keys[SDL_SCANCODE_A]) {
        angularSpeed -= Math::Pi;
    }
    if(keys[SDL_SCANCODE_D]) {
        angularSpeed += Math::Pi;
    }

    moveComp->setForwardSpeed(forwardSpeed);
    moveComp->setAngularSpeed(angularSpeed);

    // Adjst horizontal distance of camera based on speed
    if(!Math::NearZero(forwardSpeed)) {
        cameraComp->setHorzDist(500.0f);
    } else {
        cameraComp->setHorzDist(350.0f);
    }
}

void FollowActor::setVisible(bool visible) {
    meshComp->setVisible(visible);
}
