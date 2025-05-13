#include "bg_sprite_component.h"

#include "actor.h"

BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder) :
    SpriteComponent(owner, drawOrder),
    screenSize(Vector2::Zero),
    scrollSpeed(0) {}

void BGSpriteComponent::update(float deltaTime) {
    SpriteComponent::update(deltaTime);

    for(auto& bg: bgTextures) {
        // Update the x offset
        bg.offset.x += scrollSpeed * deltaTime;

        // If this is completely off the screen reset to
        // the right of tle last bg texture
        if(bg.offset.x < -screenSize.x) {
            bg.offset.x = (bgTextures.size() - 1) * screenSize.x - 2;
        }
    }
}

void BGSpriteComponent::draw(SDL_Renderer* renderer) {
    if(bgTextures.size() > 0) {
        for(auto bg: bgTextures) {
            SDL_FRect r;
            // Draw on whole screen
            r.w = screenSize.x;
            r.h = screenSize.y;
            // Center the rectangle around the position of owner
            r.x = owner->getPosition().x - r.w / 2.0f + bg.offset.x;
            r.y = owner->getPosition().x - r.w / 2.0f + bg.offset.y;

            // Draw
            SDL_RenderTexture(renderer, bg.texture, nullptr, &r);
        }
    }
}

void BGSpriteComponent::setBGTextures(const std::vector<SDL_Texture*>& textures) {
    int count = 0;
    for(auto tex: textures) {
        BGTexture temp;
        temp.texture = tex;

        // Each texture is screen with in offset
        temp.offset.x = count * screenSize.x;
        temp.offset.y = 0;

        bgTextures.emplace_back(temp);
        count++;
    }
}

void BGSpriteComponent::setScreenSize(const Vector2& size) {
    screenSize = size;
}

void BGSpriteComponent::setScrollSpeed(float speed) {
    scrollSpeed = speed;
}

float BGSpriteComponent::getScrollSpeed() const {
    return scrollSpeed;
}