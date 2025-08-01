#include "spotlight_component.h"

#include "actor.h"
#include "game.h"
#include "mesh.h"
#include "renderer.h"
#include "shader.h"
#include "vertex_array.h"

SpotlightComponent::SpotlightComponent(Actor* owner) : Component(owner) {
    angle = DEFAULT_HALF_ANGLE * 2.0f;
    owner->getGame()->getRenderer()->addSpotlight(this);
}

SpotlightComponent::~SpotlightComponent() {
    owner->getGame()->getRenderer()->removeSpotlight(this);
}

void SpotlightComponent::draw(Shader* shader, Mesh* mesh) {}