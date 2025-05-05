#include "anim_sprite_component.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder) :
    SpriteComponent(owner, drawOrder),
    currentAnimation(""),
    currentFrame(0) {}

void AnimSpriteComponent::update(float deltaTime) {
    SpriteComponent::update(deltaTime);

    if(animations.size() > 0) {
        // Update the current frame based on frame rate and delta time
        currentFrame += animations[currentAnimation].fps * deltaTime;

        // Wrap current frame if needed
        while(currentFrame >= animations.at(currentAnimation).textures.size()) {
            if(animations.at(currentAnimation).looping) {
                currentFrame -= animations.at(currentAnimation).textures.size();
            } else {
                currentFrame = animations.at(currentAnimation).textures.size() - 1;
            }
        }

        // Set the current texture
        setTexture(animations[currentAnimation].textures[static_cast<int>(currentFrame)]);
    }
}

void AnimSpriteComponent::addAnimation(
      const std::string& name, const std::vector<SDL_Texture*>& textures) {
    Animation animation{ textures, 24, true };
    animations.emplace(name, animation);
    if(animations.size() > 0) {
        // Set the active texture to first frame
        currentFrame = 0.0f;
        setTexture(animations[name].textures[0]);
    }
}

void AnimSpriteComponent::playAnimation(const std::string& name) {
    if(name != currentAnimation) {
        currentFrame = 0.0f;
    }
    currentAnimation = name;
}

float AnimSpriteComponent::getAnimFPS(const std::string& animation) const {
    return animations.at(animation).fps;
}

void AnimSpriteComponent::setAnimFPS(const std::string& animation, float fps) {
    animations[animation].fps = fps;
}

void AnimSpriteComponent::setAnimLoop(const std::string& animation, bool looping) {
    animations[animation].looping = looping;
}
