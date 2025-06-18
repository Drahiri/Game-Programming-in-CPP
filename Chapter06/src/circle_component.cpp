#include "circle_component.h"

#include "actor.h"

CircleComponent::CircleComponent(Actor* owner) : Component(owner), radius(0.0f) {}

void CircleComponent::setRadius(float newRadius) {
    radius = newRadius;
}

float CircleComponent::getRadius() const {
    return radius;
}

const Vector3& CircleComponent::getCenter() const {
    return owner->getPosition();
}

bool intersect(const CircleComponent& a, const CircleComponent& b) {
    // Calculate distance squared
    Vector3 diff = a.getCenter() - b.getCenter();
    float distSq = diff.LengthSq();

    // Calculate sum of radii squared
    float radiiSq = a.getRadius() + b.getRadius();
    radiiSq *= radiiSq;

    return distSq <= radiiSq;
}
