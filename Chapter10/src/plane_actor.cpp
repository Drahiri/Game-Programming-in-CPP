#include "plane_actor.h"

#include "box_component.h"
#include "game.h"
#include "mesh.h"
#include "mesh_component.h"
#include "renderer.h"

PlaneActor::PlaneActor(Game* game) : Actor(game) {
    setScale(10.0f);
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = getGame()->getRenderer()->getMesh("assets/Plane.gpmesh");
    mc->setMesh(mesh);
    BoxComponent* bc = new BoxComponent(this);
    bc->setObjectBox(mesh->getBox());

    game->addPlane(this);
}

PlaneActor::~PlaneActor() {
    getGame()->removeActor(this);
}