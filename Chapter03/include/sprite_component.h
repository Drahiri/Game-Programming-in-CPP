#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "component.h"

#include <SDL3/SDL.h>

class Actor;

class SpriteComponent : public Component {
public:
    // Lower draw order corresponds with further back
    SpriteComponent(Actor* owner, int drawOrder = 100);
    ~SpriteComponent();

    virtual void draw(SDL_Renderer* renderer);
    virtual void setTexture(SDL_Texture* newTexture);

    int getDrawOrder() const;
    int getTexHeight() const;
    int getTexWidth() const;

    void setVisible(bool isVisible);
    bool getVisible() const;

protected:
    // Texture to draw
    SDL_Texture* texture;
    // Draw order used for painter's algorithm
    int drawOrder;
    int texHeight;
    int texWidth;
    bool visible;
};

#endif