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
