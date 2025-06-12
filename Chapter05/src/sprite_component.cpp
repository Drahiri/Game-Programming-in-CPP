#include "sprite_component.h"

#include "actor.h"
#include "game.h"
#include "shader.h"
#include "texture.h"

#include <GL/glew.h>

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder) :
    Component(owner),
    drawOrder(drawOrder),
    texture(nullptr),
    texHeight(0),
    texWidth(0) {
    this->owner->getGame()->addSprite(this);
}

SpriteComponent::~SpriteComponent() {
    owner->getGame()->removeSprite(this);
}

void SpriteComponent::draw(Shader* shader) {
    // Scale the quad by the width/height of texture
    // TODO: change hardcodded values to texWidth/texHeight
    Matrix4 scaleMat =
          Matrix4::CreateScale(static_cast<float>(texWidth), static_cast<float>(texHeight), 1.0f);

    Matrix4 world = scaleMat * owner->getWorldTransform();

    // Set world transform
    shader->setMatrixUniform("uWorldTransform", world);

    texture->setActive();

    glDrawElements(GL_TRIANGLES, // Type of polygon/primitive to draw
          6,                     // Number of indices to index
          GL_UNSIGNED_INT,       // Type of each index
          nullptr                // Usually nullptr
    );
}

void SpriteComponent::setTexture(Texture* newTexture) {
    texture = newTexture;

    // Get width/height of texture
    texWidth = newTexture->getWidth();
    texHeight = newTexture->getHeight();
}

int SpriteComponent::getDrawOrder() const {
    return drawOrder;
}

int SpriteComponent::getTexHeight() const {
    return texHeight;
}

int SpriteComponent::getTexWidth() const {
    return texWidth;
}
