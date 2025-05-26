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

// Implement A* pathfinding
bool Grid::findPath(Tile* start, Tile* goal) {
    for(size_t i = 0; i < numRows; i++) {
        for(size_t j = 0; j < numCols; j++) {
            tiles[i][j]->g = 0.0f;
            tiles[i][j]->inOpenSet = false;
            tiles[i][j]->inClosedSet = false;
        }
    }

    std::vector<Tile*> openSet;

    // Set current node to start and add to closed set
    Tile* current = start;
    current->inClosedSet = true;

    do {
        // Add adjacent nodes to open set
        for(Tile* neighbor: current->adjacent) {
            if(neighbor->blocked) {
                continue;
            }

            // Only check nodes that aren't in the closed set
            if(!neighbor->inClosedSet) {
                if(!neighbor->inOpenSet) {
                    // Not in open set, so set parent
                    neighbor->parent = current;
                    neighbor->h = (neighbor->getPosition() - goal->getPosition()).Length();
                    // g(x) is the parent's g plus cost of traversing edge
                    neighbor->g = current->g + tileSize;
                    neighbor->f = neighbor->g + neighbor->h;
                    openSet.emplace_back(neighbor);
                    neighbor->inOpenSet = true;
                } else {
                    // Compute g(x) cost if current becomes the parent
                    float newG = current->g + tileSize;
                    if(newG < neighbor->g) {
                        // Adopt this node
                        neighbor->parent = current;
                        neighbor->g = newG;
                        // f(x) changes because g(x) changes
                        neighbor->f = neighbor->g + neighbor->h;
                    }
                }
            }
        }

        // If open set is empty, all possible paths are exhausted
        if(openSet.empty()) {
            break;
        }

        // Find lowest cost node in open set
        auto iter = std::min_element(
              openSet.begin(), openSet.end(), [](Tile* a, Tile* b) { return a->f < b->f; });
        // Set to current and move from open to closed
        current = *iter;
        openSet.erase(iter);
        current->inOpenSet = false;
        current->inClosedSet = true;
    } while(current != goal);

    return (current == goal) ? true : false;
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

    Tile* t = start->parent;
    while(t != getEndTile()) {
        t->setTileState(Tile::TileState::Path);
        t = t->parent;
    }
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