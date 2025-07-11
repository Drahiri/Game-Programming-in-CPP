#ifndef PHYS_WORLD_H
#define PHYS_WORLD_H

#include <vector>

class Actor;
class BoxComponent;
class Game;

class PhysWorld {
public:
    PhysWorld(Game* game);
    // Add/remove box components from world
    void addBox(BoxComponent* box);
    void removeBox(BoxComponent* box);

private:
    Game* game;
    std::vector<BoxComponent*> boxes;
};

#endif