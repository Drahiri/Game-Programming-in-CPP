#include "phys_world.h"

#include "box_component.h"
#include "game.h"

#include <algorithm>

PhysWorld::PhysWorld(Game* game) : game(game) {}

void PhysWorld::addBox(BoxComponent* box) {
    boxes.emplace_back(box);
}

void PhysWorld::removeBox(BoxComponent* box) {
    auto iter = std::find(boxes.begin(), boxes.end(), box);
    if(iter != boxes.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, boxes.end() - 1);
        boxes.pop_back();
    }
}

bool PhysWorld::segmentCast(const LineSegment& l, CollisionInfo& outColl) {
    bool collided = false;
    // Initialize closestT to infinity so first
    // intersection will always update closest T
    float closestT = Math::Infinity;
    Vector3 norm;

    // Test against all boxes
    for(auto box: boxes) {
        float t;
        // Does the segment intersect with the box?
        if(intersect(l, box->getWorldBox(), t, norm)) {
            // Is this closer than previous intersection?
            if(t < closestT) {
                outColl.point = l.pointOnSegment(t);
                outColl.normal = norm;
                outColl.box = box;
                outColl.actor = box->getOwner();
                collided = true;
            }
        }
    }

    return collided;
}

void PhysWorld::testPairwise(std::function<void(Actor*, Actor*)> f) {
    // Naive implementation O(n^2)
    for(size_t i = 0; i < boxes.size(); i++) {
        // Don't need to test vs. itself and any previous
        for(size_t j = i + 1; j < boxes.size(); j++) {
            BoxComponent* a = boxes[i];
            BoxComponent* b = boxes[j];
            // Call supplied function to handle intersections
            f(a->getOwner(), b->getOwner());
        }
    }
}
