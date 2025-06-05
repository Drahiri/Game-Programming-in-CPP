#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "component.h"

#include <SDL3/SDL.h>

class Actor;
class Shader;

class SpriteComponent : public Component {
public:
    // Lower draw order corresponds with further back
    SpriteComponent(Actor* owner, int drawOrder = 100);
    ~SpriteComponent();

    virtual void draw(Shader* shader);
    virtual void setTexture(SDL_Texture* newTexture);

    int getDrawOrder() const;
    int getTexHeight() const;
    int getTexWidth() const;

protected:
    // Texture to draw
    SDL_Texture* texture;
    // Draw order used for painter's algorithm
    int drawOrder;
    int texHeight;
    int texWidth;
};

#endif