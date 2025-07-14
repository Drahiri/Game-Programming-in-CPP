#include "phys_world.h"

#include "box_component.h"
#include "game.h"

#include <algorithm>
#include <utility>

PhysWorld::PhysWorld(Game* game) : game(game) {}

void PhysWorld::addBox(BoxComponent* box) {
    boxesX.emplace_back(box);
    boxesY.emplace_back(box);
    boxesZ.emplace_back(box);
}

void PhysWorld::removeBox(BoxComponent* box) {
    auto iter = std::find(boxesX.begin(), boxesX.end(), box);
    if(iter != boxesX.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, boxesX.end() - 1);
        boxesX.pop_back();
    }

    iter = std::find(boxesY.begin(), boxesY.end(), box);
    if(iter != boxesY.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, boxesY.end() - 1);
        boxesY.pop_back();
    }
    iter = std::find(boxesZ.begin(), boxesZ.end(), box);
    if(iter != boxesZ.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, boxesZ.end() - 1);
        boxesZ.pop_back();
    }
}

bool PhysWorld::segmentCast(const LineSegment& l, CollisionInfo& outColl) {
    bool collided = false;
    // Initialize closestT to infinity so first
    // intersection will always update closest T
    float closestT = Math::Infinity;
    Vector3 norm;

    // Test against all boxes
    for(auto box: boxesX) {
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
                closestT = t;
            }
        }
    }

    return collided;
}

void PhysWorld::testPairwise(std::function<void(Actor*, Actor*)> f) {
    // Naive implementation O(n^2)
    for(size_t i = 0; i < boxesX.size(); i++) {
        // Don't need to test vs. itself and any previous
        for(size_t j = i + 1; j < boxesX.size(); j++) {
            BoxComponent* a = boxesX[i];
            BoxComponent* b = boxesX[j];
            // Call supplied function to handle intersections
            f(a->getOwner(), b->getOwner());
        }
    }
}

void PhysWorld::testSweepAndPrune(std::function<void(Actor*, Actor*)> f) {
    // Sort boxesX by min.x
    std::sort(boxesX.begin(), boxesX.end(), [](BoxComponent* a, BoxComponent* b) {
        return a->getWorldBox().min.x < b->getWorldBox().min.x;
    });

    std::vector<std::pair<BoxComponent*, BoxComponent*>> possibleCollisionX;

    for(size_t i = 0; i < boxesX.size(); i++) {
        // Get max.x for box[i]
        BoxComponent* a = boxesX[i];
        float max = a->getWorldBox().max.x;

        for(size_t j = i + 1; j < boxesX.size(); j++) {
            BoxComponent* b = boxesX[j];
            // If box[j] min.x is past the max.x bounds
            // then there aren't any other possible itersection
            // against box [i]
            if(b->getWorldBox().min.x > max) {
                break;
            } else {
                std::pair<BoxComponent*, BoxComponent*> pair{ a, b };
                possibleCollisionX.emplace_back(pair);
            }
        }
    }

    // Sort boxesY by min.y
    std::sort(boxesY.begin(), boxesY.end(), [](BoxComponent* a, BoxComponent* b) {
        return a->getWorldBox().min.y < b->getWorldBox().min.y;
    });

    std::vector<std::pair<BoxComponent*, BoxComponent*>> possibleCollisionY;

    for(size_t i = 0; i < boxesY.size(); i++) {
        // Get max.y for box[i]
        BoxComponent* a = boxesY[i];
        float max = a->getWorldBox().max.y;

        for(size_t j = i + 1; j < boxesY.size(); j++) {
            BoxComponent* b = boxesY[j];
            // If box[j] min.y is past the max.y bounds
            // then there aren't any other possible itersection
            // against box [i]
            if(b->getWorldBox().min.y > max) {
                break;
            } else {
                std::pair<BoxComponent*, BoxComponent*> pair{ a, b };
                possibleCollisionY.emplace_back(pair);
            }
        }
    }

    // Sort boxesZ by min.z
    std::sort(boxesZ.begin(), boxesZ.end(), [](BoxComponent* a, BoxComponent* b) {
        return a->getWorldBox().min.z < b->getWorldBox().min.z;
    });

    std::vector<std::pair<BoxComponent*, BoxComponent*>> possibleCollisionZ;

    for(size_t i = 0; i < boxesZ.size(); i++) {
        // Get max.y for box[i]
        BoxComponent* a = boxesZ[i];
        float max = a->getWorldBox().max.z;

        for(size_t j = i + 1; j < boxesZ.size(); j++) {
            BoxComponent* b = boxesZ[j];
            // If box[j] min.z is past the max.z bounds
            // then there aren't any other possible itersection
            // against box [i]
            if(b->getWorldBox().min.z > max) {
                break;
            } else {
                std::pair<BoxComponent*, BoxComponent*> pair{ a, b };
                possibleCollisionZ.emplace_back(pair);
            }
        }
    }

    // Find collisions on XY axes
    std::vector<std::pair<BoxComponent*, BoxComponent*>> possibleCollisionXY;
    for(size_t i = 0; i < possibleCollisionX.size(); i++) {
        auto& pair = possibleCollisionX[i];
        auto iter = std::find(possibleCollisionY.begin(), possibleCollisionY.end(), pair);
        if(iter != possibleCollisionY.end()) {
            possibleCollisionXY.emplace_back(pair);
        }
    }

    // Find collisions on XY axes with Z axes
    for(size_t i = 0; i < possibleCollisionXY.size(); i++) {
        auto& pair = possibleCollisionXY[i];
        auto iter = std::find(possibleCollisionZ.begin(), possibleCollisionXY.end(), pair);
        if(iter != possibleCollisionZ.end()) {
            f(pair.first->getOwner(), pair.second->getOwner());
        }
    }
}
