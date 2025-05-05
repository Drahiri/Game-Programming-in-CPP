#ifndef ANIM_SPRITE_COMPONENT_H
#define ANIM_SPRITE_COMPONENT_H

#include "sprite_component.h"

#include <map>
#include <string>
#include <vector>

class Actor;

class AnimSpriteComponent : public SpriteComponent {
public:
    AnimSpriteComponent(Actor* owner, int drawOrder = 100);
    // Update animation every frame (overriden from component)
    void update(float deltaTime) override;
    // Set the textures used for animation
    void addAnimation(const std::string& name, const std::vector<SDL_Texture*>& textures);
    void playAnimation(const std::string& name);
    float getAnimFPS(const std::string& animation) const;
    void setAnimFPS(const std::string& animation, float fps);

private:
    struct Animation {
        std::vector<SDL_Texture*> textures;
        float fps;
    };

    // All textures in the animation
    std::map<std::string, Animation> animations;

    // Current played animation
    std::string currentAnimation;
    // Current frame displayed
    float currentFrame;
};

#endif