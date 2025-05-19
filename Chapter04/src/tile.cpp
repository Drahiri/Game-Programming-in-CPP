#include "tile.h"

#include "game.h"
#include "sprite_component.h"

Tile::Tile(Game* game) :
    Actor(game),
    parent(nullptr),
    f(0.0f),
    g(0.0f),
    h(0.0f),
    blocked(false),
    sprite(nullptr),
    tileState(TileState::Default),
    selected(false) {
    sprite = new SpriteComponent(this);
    updateTexture();
}

void Tile::setTileState(TileState state) {
    tileState = state;
    updateTexture();
}

Tile::TileState Tile::getTileState() const {
    return tileState;
}

void Tile::toggleSelect() {
    selected = !selected;
    updateTexture();
}

const Tile* Tile::getParent() const {
    return parent;
}

void Tile::updateTexture() {
    std::string text;
    switch(tileState) {
    case TileState::Start:
        text = "assets/TileTan.png";
        break;
    case TileState::Base:
        text = "assets/TileGreen.png";
        break;
    case TileState::Path:
        if(selected) {
            text = "assets/TileGreySelected.png";
        } else {
            text = "assets/TileGrey.png";
        }
        break;
    case TileState::Default:
    default:
        if(selected) {
            text = "assets/TileBrownSelected.png";
        } else {
            text = "assets/TileBrown.png";
        }
        break;
    }
    sprite->setTexture(getGame()->getTexture(text));
}
