#include "sprite_component.h"

#include "actor.h"
#include "game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder) :
    Component(owner),
    drawOrder(drawOrder),
    texture(nullptr),
    texHeight(0),
    texWidth(0),
    visible(true) {
    this->owner->getGame()->addSprite(this);
}

SpriteComponent::~SpriteComponent() {
    owner->getGame()->removeSprite(this);
}

void SpriteComponent::draw(SDL_Renderer* renderer) {
    if(texture && visible) {
        SDL_FRect r;
        // Scale the width/height by owner's scale
        r.w = texWidth * owner->getScale();
        r.h = texHeight * owner->getScale();

        // Center the rectangle around the position of owner
        r.x = owner->getPosition().x - r.w / 2.0f;
        r.y = owner->getPosition().y - r.h / 2.0f;

        // Draw
        SDL_RenderTextureRotated(renderer,
              texture,
              nullptr,
              &r,
              -Math::ToDegrees(owner->getRotation()),
              nullptr,
              SDL_FLIP_NONE);
    }
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

void SpriteComponent::setVisible(bool isVisible) {
    visible = isVisible;
}

bool SpriteComponent::getVisible() const {
    return visible;
}