#include "point_light_component.h"

#include "actor.h"
#include "game.h"
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
    shader->setVec3Uniform(
          "uPointLight.specColor", diffuseColor * 1.5); // 1.5 time brighter than diffuse color

    Matrix4 invView = owner->getGame()->getRenderer()->getView();
    invView.Invert();

    shader->setVec3Uniform("uCameraPos", invView.GetTranslation());

    // Draw the sphere
    glDrawElements(GL_TRIANGLES, mesh->getVertexArray()->getNumIndices(), GL_UNSIGNED_INT, nullptr);
}