#include "mesh_component.h"

#include "actor.h"
#include "game.h"
#include "level_loader.h"
#include "mesh.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"

MeshComponent::MeshComponent(Actor* owner, bool skeletal) :
    Component(owner),
    mesh(nullptr),
    textureIndex(0),
    isVisible(true),
    isSkeletal(skeletal) {
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

        // Set specular power
        shader->setFloatUniform("uSpecPower", mesh->getSpecPower());

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

void MeshComponent::setVisible(bool visible) {
    isVisible = visible;
}

bool MeshComponent::getVisible() const {
    return isVisible;
}

bool MeshComponent::getIsSkeletal() const {
    return isSkeletal;
}

Component::TypeID MeshComponent::getType() const {
    return Component::TypeID::MeshComponent;
}

void MeshComponent::loadProperties(const rapidjson::Value& inObject) {
    Component::loadProperties(inObject);

    std::string meshFile;
    if(JsonHelper::getString(inObject, "meshFile", meshFile)) {
        setMesh(owner->getGame()->getRenderer()->getMesh(meshFile));
    }

    int idx;
    if(JsonHelper::getInt(inObject, "textureIndex", idx)) {
        textureIndex = static_cast<size_t>(idx);
    }

    JsonHelper::getBool(inObject, "visible", isVisible);
    JsonHelper::getBool(inObject, "isSkeletal", isSkeletal);
}