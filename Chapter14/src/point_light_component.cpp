#include "point_light_component.h"

#include "actor.h"
#include "game.h"
#include "level_loader.h"
#include "mesh.h"
#include "renderer.h"
#include "shader.h"
#include "vertex_array.h"

PointLightComponent::PointLightComponent(Actor* owner) : Component(owner) {
    owner->getGame()->getRenderer()->addPointLight(this);
}

PointLightComponent::~PointLightComponent() {
    owner->getGame()->getRenderer()->removePointLight(this);
}

void PointLightComponent::draw(Shader* shader, Mesh* mesh) {
    // Scale world transform to the outer radius (divide by
    // the mesh radius) and positioned to the world position
    Matrix4 scale = Matrix4::CreateScale(owner->getScale() * outerRadius / mesh->getRadius());
    Matrix4 trans = Matrix4::CreateTranslation(owner->getPosition());
    Matrix4 worldTransform = scale * trans;
    shader->setMatrixUniform("uWorldTransform", worldTransform);
    // Set point light shader constants
    shader->setVec3Uniform("uPointLight.worldPos", owner->getPosition());
    shader->setVec3Uniform("uPointLight.diffuseColor", diffuseColor);
    shader->setFloatUniform("uPointLight.innerRadius", innerRadius);
    shader->setFloatUniform("uPointLight.outerRadius", outerRadius);

    // Draw the sphere
    glDrawElements(GL_TRIANGLES, mesh->getVertexArray()->getNumIndices(), GL_UNSIGNED_INT, nullptr);
}

Component::TypeID PointLightComponent::getType() const {
    return Component::TypeID::PointLightComponent;
}

void PointLightComponent::loadProperties(const rapidjson::Value& inObject) {
    Component::loadProperties(inObject);

    JsonHelper::getVector3(inObject, "color", diffuseColor);
    JsonHelper::getFloat(inObject, "innerRadius", innerRadius);
    JsonHelper::getFloat(inObject, "outerRadius", outerRadius);
}

void PointLightComponent::saveProperties(
      rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject) const {
    Component::saveProperties(alloc, inObject);

    JsonHelper::addVector3(alloc, inObject, "color", diffuseColor);
    JsonHelper::addFloat(alloc, inObject, "innerRadius", innerRadius);
    JsonHelper::addFloat(alloc, inObject, "outerRadius", outerRadius);
}