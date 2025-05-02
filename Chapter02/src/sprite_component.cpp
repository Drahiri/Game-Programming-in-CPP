#include "sprite_component.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder) :
    Component(owner),
    drawOrder(drawOrder),
    texture(nullptr),
    texHeight(0),
    texWidth(0) {}

SpriteComponent::~SpriteComponent() {}

void SpriteComponent::draw(SDL_Renderer* renderer) {}

void SpriteComponent::setTexture(SDL_Texture* newTexture) {
    texture = newTexture;
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
