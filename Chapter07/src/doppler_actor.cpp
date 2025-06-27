#include "doppler_actor.h"

#include "game.h"
#include "move_component.h"

DopplerActor::DopplerActor(Game* game) : Actor(game) {
    mc = new MoveComponent(this);
}

void DopplerActor::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    time += deltaTime;

    Vector3 dir = Vector3(Math::Sin(time), -Math::Cos(time), 0.0f);
    dir.Normalize();

    mc->addForce(dir * 2000.0f);
}