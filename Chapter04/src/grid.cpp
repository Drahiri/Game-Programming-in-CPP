#include "grid.h"

#include "enemy.h"
#include "tile.h"
#include "tower.h"

#include <algorithm>

Grid::Grid(Game* game) : Actor(game), selectedTile(nullptr) {
    // 7 rows, 16 columns
    tiles.resize(numRows);
    for(size_t i = 0; i < tiles.size(); i++) {
        tiles[i].resize(numCols);
    }

    // Create tiles
    for(size_t i = 0; i < numRows; i++) {
        for(size_t j = 0; j < numCols; j++) {
            tiles[i][j] = new Tile(getGame());
            tiles[i][j]->setPosition(
                  Vector2(tileSize / 2.0f + j * tileSize, startY + i * tileSize));
        }
    }

    // Set start/end tiles
    getStartTile()->setTileState(Tile::TileState::Start);
    getEndTile()->setTileState(Tile::TileState::Base);

    // Set up adjacency lists
    for(size_t i = 0; i < numRows; i++) {
        for(size_t j = 0; j < numCols; j++) {
            if(i > 0) {
                tiles[i][j]->adjacent.push_back(tiles[i - 1][j]);
            }
            if(i < numRows - 1) {
                tiles[i][j]->adjacent.push_back(tiles[i + 1][j]);
            }
            if(j > 0) {
                tiles[i][j]->adjacent.push_back(tiles[i][j - 1]);
            }
            if(j < numCols - 1) {
                tiles[i][j]->adjacent.push_back(tiles[i][j + 1]);
            }
        }
    }

    // Find path (in reverse)
    findPath(getEndTile(), getStartTile());
    updatePathTiles(getStartTile());

    nextEnemy = enemyTime;
}

void Grid::selectTile(size_t row, size_t col) {
    // Make sure it's a valid selection
    Tile::TileState tstate = tiles[row][col]->getTileState();
    if(tstate != Tile::TileState::Start && tstate != Tile::TileState::Base) {
        // Deselect previous one
        if(selectedTile) {
            selectedTile->toggleSelect();
        }
        selectedTile = tiles[row][col];
        selectedTile->toggleSelect();
    }
}

void Grid::processClick(int x, int y) {
    y -= static_cast<int>(startY - tileSize / 2);
    if(y >= 0) {
        x /= static_cast<int>(tileSize);
        y /= static_cast<int>(tileSize);
        if(x >= 0 && static_cast<int>(numCols) && y >= 0 && y < static_cast<int>(numRows)) {
            selectTile(y, x);
        }
    }
}

bool Grid::findPath(Tile* start, Tile* goal) {
    return false;
}

void Grid::updatePathTiles(Tile* start) {
    // Reset all tiles to normal (except for start/end)
    for(size_t i = 0; i < numRows; i++) {
        for(size_t j = 0; j < numCols; j++) {
            if(!(i == 3 && j == 0) && !(i == 3 && j == 15)) {
                tiles[i][j]->setTileState(Tile::TileState::Default);
            }
        }
    }

    // Tile* t = start->parent;
    // while(t != getEndTile()) {
    //     t->setTileState(Tile::TileState::Path);
    //     t = t->parent;
    // }
}

void Grid::buildTower() {
    if(selectedTile && !selectedTile->blocked) {
        selectedTile->blocked = true;
        if(findPath(getEndTile(), getStartTile())) {
            Tower* t = new Tower(getGame());
            t->setPosition(selectedTile->getPosition());
        } else {
            // This tower would block the path, so don't allow build
            selectedTile->blocked = false;
            findPath(getEndTile(), getStartTile());
        }
        updatePathTiles(getStartTile());
    }
}

Tile* Grid::getStartTile() {
    return tiles[3][0];
}

Tile* Grid::getEndTile() {
    return tiles[3][15];
}

void Grid::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    // Is it time to spawn a new enemy?
    nextEnemy -= deltaTime;
    if(nextEnemy <= 0.0f) {
        new Enemy(getGame());
        nextEnemy += enemyTime;
    }
}