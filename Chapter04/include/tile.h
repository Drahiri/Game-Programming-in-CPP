#ifndef TILE_H
#define TILE_H

#include "actor.h"

#include <vector>

class Game;
class Grid;
class SpriteComponent;

class Tile : public Actor {
public:
    friend Grid;
    enum class TileState {
        Default,
        Path,
        Start,
        Base
    };

    Tile(Game* game);

    void setTileState(TileState state);
    TileState getTileState() const;
    void toggleSelect();
    const Tile* getParent() const;

private:
    // For pathfinding
    std::vector<Tile*> adjacent;
    Tile* parent;
    float f;
    float g;
    float h;
    bool inOpenSet;
    bool inClosedSet;
    bool blocked;

    void updateTexture();
    SpriteComponent* sprite;
    TileState tileState;
    bool selected;
};

#endif