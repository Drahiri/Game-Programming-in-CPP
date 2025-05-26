#include "nav_component.h"

#include "actor.h"
#include "tile.h"

NavComponent::NavComponent(Actor* owner, int updateOrder) :
    MoveComponent(owner, updateOrder),
    nextPoint(nullptr) {}

void NavComponent::update(float deltaTime) {
    if(nextPoint) {
        // If you've reached the next point, advance along
        Vector2 diff = owner->getPosition() - nextPoint->getPosition();

        if(diff.Length() <= 2.0f) {
            nextPoint = nextPoint->getParent();
            turnTo(nextPoint->getPosition());
        }
    }

    // This moves the actor forward
    MoveComponent::update(deltaTime);
}

void NavComponent::startPath(const Tile* start) {
    nextPoint = start->getParent();
    turnTo(nextPoint->getPosition());
}

void NavComponent::turnTo(const Vector2& pos) {
    // Vector from me to pos
    Vector2 dir = pos - owner->getPosition();

    // New angle is just atan2 of this dir vector
    // (Negate y because +y is down on screen)
    float angle = Math::Atan2(-dir.y, dir.x);
    owner->setRotation(angle);
}
