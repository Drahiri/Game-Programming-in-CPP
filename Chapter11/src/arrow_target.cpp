#include "arrow_target.h"

#include "game.h"
#include "mesh.h"
#include "mesh_component.h"
#include "renderer.h"

ArrowTarget::ArrowTarget(Game* game) : Actor(game) {
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = getGame()->getRenderer()->getMesh("assets/Cube.gpmesh");
    mc->setMesh(mesh);
}