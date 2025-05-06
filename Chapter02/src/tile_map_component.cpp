#include "tile_map_component.h"

#include <fstream>
#include <sstream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder) :
    SpriteComponent(owner, drawOrder),
    tileSize(Vector2::Zero) {}

void TileMapComponent::draw(SDL_Renderer* renderer) {}

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
}
