#include "mesh_component.h"

#include "actor.h"
#include "game.h"
#include "mesh.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"

MeshComponent::MeshComponent(Actor* owner) : Component(owner) {
    owner->getGame()->getRenderer()->addMeshComp(this);
}

MeshComponent::~MeshComponent() {
    owner->getGame()->getRenderer()->removeMeshComp(this);
}

void MeshComponent::draw(Shader* shader) {
    if(mesh) {
        // Set the world transform
        shader->setMatrixUniform("uWorldTransform", owner->getWorldTransform());

        // Set the active texture
        Texture* t = mesh->getTexture(textureIndex);
        if(t) {
            t->setActive();
        }

        // Set the mesh's vertex array as active
        VertexArray* va = mesh->getVertexArray();
        va->setActive();

        // Draw
        glDrawElements(GL_TRIANGLES, va->getNumIndices(), GL_UNSIGNED_INT, nullptr);
    }
}

void MeshComponent::setMesh(Mesh* mesh) {
    this->mesh = mesh;
}

void MeshComponent::setTextureIndex(size_t index) {
    textureIndex = index;
}
