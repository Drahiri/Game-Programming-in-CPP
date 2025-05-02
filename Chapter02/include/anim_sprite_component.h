#ifndef ANIM_SPRITE_COMPONENT_H
#define ANIM_SPRITE_COMPONENT_H

#include "sprite_component.h"

#include <vector>

class Actor;

class AnimSpriteComponent : public SpriteComponent {
public:
    AnimSpriteComponent(Actor* owner, int drawOrder = 100);
    // Update animation every frame (overriden from component)
    void update(float deltaTime) override;
    // Set the textures used for animation
    void setAnimTextures(const std::vector<SDL_Texture*>& textures);
    float getAnimFPS() const;
    void setAnimFPS(float fps);

private:
    // All textures in the animation
    std::vector<SDL_Texture*> animTextures;
    // Current frame displayed
    float currentFrame;
    // Animation frame rate
    float animFPS;
};

#endif