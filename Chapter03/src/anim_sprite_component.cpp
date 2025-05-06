#include "anim_sprite_component.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder) :
    SpriteComponent(owner, drawOrder),
    currentFrame(0),
    animFPS(24) {}

void AnimSpriteComponent::update(float deltaTime) {
    SpriteComponent::update(deltaTime);

    if(animTextures.size() > 0) {
        // Update the current frame based on frame rate and delta time
        currentFrame += animFPS * deltaTime;

        // Wrap current frame if needed
        while(currentFrame >= animTextures.size()) {
            currentFrame -= animTextures.size();
        }

        // Set the current texture
        setTexture(animTextures[static_cast<int>(currentFrame)]);
    }
}

void AnimSpriteComponent::setAnimTextures(const std::vector<SDL_Texture*>& textures) {
    animTextures = textures;
    if(animTextures.size() > 0) {
        // Set the active texture to first frame
        currentFrame = 0.0;
        setTexture(textures[0]);
    }
}

float AnimSpriteComponent::getAnimFPS() const {
    return animFPS;
}

void AnimSpriteComponent::setAnimFPS(float fps) {
    animFPS = fps;
}
