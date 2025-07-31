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