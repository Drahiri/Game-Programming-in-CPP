#include "ball_actor.h"

#include "audio_component.h"
#include "ball_move.h"
#include "game.h"
#include "mesh.h"
#include "mesh_component.h"
#include "renderer.h"

BallActor::BallActor(Game* game) : Actor(game), lifeSpan(2.0f) {
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = getGame()->getRenderer()->getMesh("assets/Sphere.gpmesh");
    mc->setMesh(mesh);
    myMove = new BallMove(this);
    myMove->setForwardSpeed(1500.0f);
    audioComp = new AudioComponent(this);
}

void BallActor::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    lifeSpan -= deltaTime;
    if(lifeSpan < 0.0f) {
        setState(State::Dead);
    }
}

void BallActor::setPlayer(Actor* player) {
    myMove->setPlayer(player);
}

void BallActor::hitTarget() {
    audioComp->playEvent("event:/Ding");
}
