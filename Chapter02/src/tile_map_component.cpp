#include "tile_map_component.h"

#include "actor.h"

#include <fstream>
#include <sstream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder) :
    SpriteComponent(owner, drawOrder),
    tileSize(Vector2::Zero),
    tilesInTextureWidth(0) {}

void TileMapComponent::draw(SDL_Renderer* renderer) {
    if(texture) {
        SDL_FRect dstRect;
        // Scale the tile width/height by owner's scale
        dstRect.w = tileSize.x * owner->getScale();
        dstRect.h = tileSize.y * owner->getScale();

        // Always draw from beginning of screen
        dstRect.x = 0;
        dstRect.y = 0;

        SDL_FRect srcRect;
        srcRect.w = tileSize.x;
        srcRect.h = tileSize.y;

        // Draw all layers
        for(int layer = 0; layer < layers.size(); layer++) {
            for(int line = 0; line < layers[layer].size(); line++) {
                for(int tile = 0; tile < layers[layer][line].size(); tile++) {
                    int currentTile = layers[layer][line][tile];
                    if(currentTile >= 0) {
                        srcRect.x = currentTile % tilesInTextureWidth * tileSize.x;
                        srcRect.y = currentTile / tilesInTextureWidth * tileSize.y;
                        SDL_RenderTexture(renderer, texture, &srcRect, &dstRect);
                    }

                    dstRect.x += dstRect.w;
                }
                dstRect.x = 0;
                dstRect.y += dstRect.h;
            }
            dstRect.y = 0;
        }
    }
}

void TileMapComponent::addTileLayer(const std::string& layerFilename) {
    // Read file
    std::ifstream file;
    file.open(layerFilename);
    if(!file.is_open()) {
        SDL_Log("Failed to open tile map layer '%s'", layerFilename.c_str());
        return;
    }

    // Read line by line
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    std::string line;
    std::vector<std::vector<int>> layer;
    while(getline(ss, line)) {
        std::stringstream lineStream{ line };
        std::vector<int> lineVector;
        std::string tile;

        while(getline(lineStream, tile, ',')) {
            lineVector.push_back(stoi(tile));
        }
        layer.push_back(lineVector);
    }
    layers.push_back(layer);
}

void TileMapComponent::setTileSize(const Vector2& newTileSize) {
    tileSize = newTileSize;
    tilesInTextureWidth = static_cast<int>(texWidth / tileSize.x);
}
