#include "ball_actor.h"

#include "ball_move.h"
#include "game.h"
#include "mesh.h"
#include "mesh_component.h"
#include "renderer.h"

BallActor::BallActor(Game* game) : Actor(game) {
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = getGame()->getRenderer()->getMesh("assets/Sphere.gpmesh");
    mc->setMesh(mesh);
    myMove = new BallMove(this);
    myMove->setForwardSpeed(1500.0f);
}

void BallActor::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);
}
