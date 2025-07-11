#ifndef PHYS_WORLD_H
#define PHYS_WORLD_H

#include "collision.h"
#include "math.h"

#include <functional>
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

    // Test collisions using naive pairwise
    void testPairwise(std::function<void(Actor*, Actor*)> f);
    // Test collisions using sweep and prune
    void testSweepAndPrune(std::function<void(Actor*, Actor*)> f);

private:
    Game* game;
    std::vector<BoxComponent*> boxes;
};

#endif