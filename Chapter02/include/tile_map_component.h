#ifndef TILE_MAP_COMPONENT_H
#define TILE_MAP_COMPONENT_H

#include "math.h"
#include "sprite_component.h"

#include <string>
#include <vector>

class Actor;

class TileMapComponent : public SpriteComponent {
public:
    TileMapComponent(Actor* owner, int drawOrder = 100);

    void draw(SDL_Renderer* renderer) override;

    void addTileLayer(const std::string& layerFilename);

    void setTileSize(const Vector2& newTileSize);

private:
    Vector2 tileSize;
    int tilesInTextureWidth;
    std::vector<std::vector<std::vector<int>>> layers;
};

#endif