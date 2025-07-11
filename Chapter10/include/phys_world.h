#ifndef PHYS_WORLD_H
#define PHYS_WORLD_H

#include "collision.h"
#include "math.h"

#include <vector>

class Actor;
class BoxComponent;
class Game;

class PhysWorld {
public:
    PhysWorld(Game* game);

    // Used to give helpful information about collision result
    struct CollisionInfo {
        // Point of collision
        Vector3 point;
        // Normal at collision
        Vector3 normal;
        // Component collided with
        BoxComponent* box;
        // Owning actor of component
        Actor* actor;
    };

    // Add/remove box components from world
    void addBox(BoxComponent* box);
    void removeBox(BoxComponent* box);

    bool segmentCast(const LineSegment& l, CollisionInfo& outColl);

private:
    Game* game;
    std::vector<BoxComponent*> boxes;
};

#endif