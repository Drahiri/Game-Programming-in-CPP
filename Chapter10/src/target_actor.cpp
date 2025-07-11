#include "target_actor.h"

#include "box_component.h"
#include "game.h"
#include "mesh.h"
#include "mesh_component.h"
#include "renderer.h"

TargetActor::TargetActor(Game* game) : Actor(game) {
    setRotation(Quaternion(Vector3::UnitZ, Math::Pi));
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = getGame()->getRenderer()->getMesh("assets/Target.gpmesh");
    mc->setMesh(mesh);
    // Add collision box
    BoxComponent* bc = new BoxComponent(this);
    bc->setObjectBox(mesh->getBox());
}