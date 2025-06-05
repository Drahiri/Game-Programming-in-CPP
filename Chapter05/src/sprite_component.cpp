#include "sprite_component.h"

#include "actor.h"
#include "game.h"

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
    glDrawElements(GL_TRIANGLES, // Type of polygon/primitive to draw
          6,                     // Number of indices to index
          GL_UNSIGNED_INT,       // Type of each index
          nullptr                // Usually nullptr
    );
}

void SpriteComponent::setTexture(SDL_Texture* newTexture) {
    texture = newTexture;

    // Get width/height of texture
    float w = 0.0f;
    float h = 0.0f;
    SDL_GetTextureSize(texture, &w, &h);
    texWidth = static_cast<int>(w);
    texHeight = static_cast<int>(h);
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
