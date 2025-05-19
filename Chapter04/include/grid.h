#ifndef GRID_H
#define GRID_H

#include "actor.h"

#include <vector>

class Game;
class Tile;

class Grid : public Actor {
public:
    Grid(Game* game);

    // Handle a mouse click at the x/y screen locations
    void processClick(int x, int y);

    // Use A* to find a path
    bool findPath(Tile* start, Tile* goal);

    // Try to build a tower
    void buildTower();

    // Get start/end tile
    Tile* getStartTile();
    Tile* getEndTile();

    void updateActor(float deltaTime) override;

private:
    // Select a specific tile
    void selectTile(size_t row, size_t col);

    // Update textures for tiles on path
    void updatePathTiles(Tile* start);

    // Currently selected tile
    Tile* selectedTile;

    // 2D vector of tiles in grid
    std::vector<std::vector<Tile*>> tiles;

    // Time until next enemy
    float nextEnemy;

    // Rows/columns in grid
    const size_t numRows = 7;
    const size_t numCols = 16;
    // Start y position of top left corner
    const float startY = 192.0f;
    // Width/height of each tile
    const float tileSize = 64.0f;
    // Time between enemies
    const float enemyTime = 1.5f;
};

#endif