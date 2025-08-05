#include "follow_actor.h"

#include "follow_camera.h"
#include "game.h"
#include "level_loader.h"
#include "mirror_camera.h"
#include "move_component.h"
#include "renderer.h"
#include "skeletal_mesh_component.h"

FollowActor::FollowActor(Game* game) : Actor(game), moving(false) {
    meshComp = new SkeletalMeshComponent(this);
    meshComp->setMesh(game->getRenderer()->getMesh("assets/CatWarrior.gpmesh"));
    meshComp->setSkeleton(game->getSkeleton("assets/CatWarrior.gpskel"));
    meshComp->playAnimation(game->getAnimation("assets/CatActionIdle.gpanim"));

    setPosition(Vector3(0.0f, 0.0f, -100.0f));

    moveComp = new MoveComponent(this);
    cameraComp = new FollowCamera(this);
    cameraComp->snapToIdeal();

    // Add a component for mirror camera
    MirrorCamera* mirror = new MirrorCamera(this);
    mirror->snapToIdeal();
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

    // Did we start moving?
    if(!moving && !Math::NearZero(forwardSpeed)) {
        moving = true;
        meshComp->playAnimation(getGame()->getAnimation("assets/CatRunSprint.gpanim"), 1.25f);
    } else if(moving && Math::NearZero(forwardSpeed)) {
        moving = false;
        meshComp->playAnimation(getGame()->getAnimation("assets/CatActionIdle.gpanim"));
    }
}

void FollowActor::setVisible(bool visible) {
    meshComp->setVisible(visible);
}

void FollowActor::loadProperties(const rapidjson::Value& inObj) {
    Actor::loadProperties(inObj);
    JsonHelper::getBool(inObj, "moving", moving);
}

void FollowActor::saveProperties(
      rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    Actor::saveProperties(alloc, inObj);
    JsonHelper::addBool(alloc, inObj, "moving", moving);
}

Actor::TypeID FollowActor::getType() const {
    return TypeID::FollowActor;
}