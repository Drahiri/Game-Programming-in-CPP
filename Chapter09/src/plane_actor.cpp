#include "plane_actor.h"

#include "game.h"
#include "mesh_component.h"
#include "renderer.h"

PlaneActor::PlaneActor(Game* game) : Actor(game) {
    setScale(10.0f);
    MeshComponent* mc = new MeshComponent(this);
    mc->setMesh(getGame()->getRenderer()->getMesh("assets/Plane.gpmesh"));
}