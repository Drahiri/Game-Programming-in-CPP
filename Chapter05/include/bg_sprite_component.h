#ifndef BG_SPRITE_COMPONENT_H
#define BG_SPRITE_COMPONENT_H

#include "math.h"
#include "sprite_component.h"

#include <vector>

class Actor;

class BGSpriteComponent : public SpriteComponent {
public:
    // Set draw order default to lower (so it's in the background)
    BGSpriteComponent(Actor* owner, int drawOrder = 10);
    // Update/draw overriden from parent
    void update(float deltaTime) override;
    void draw(SDL_Renderer* renderer) override;
    // Set the textures used for the background
    void setBGTextures(const std::vector<SDL_Texture*>& textures);
    // Get/set screen size and scroll speed
    void setScreenSize(const Vector2& size);
    void setScrollSpeed(float speed);
    float getScrollSpeed() const;

private:
    struct BGTexture {
        SDL_Texture* texture;
        Vector2 offset;
    };

    std::vector<BGTexture> bgTextures;
    Vector2 screenSize;
    float scrollSpeed;
};

#endif