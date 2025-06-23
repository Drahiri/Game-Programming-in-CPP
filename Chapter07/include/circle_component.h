#ifndef CIRCLE_COMPONENT_H
#define CIRCLE_COMPONENT_H

#include "component.h"
#include "math.h"

class Actor;

class CircleComponent : public Component {
public:
    CircleComponent(Actor* owner);

    void setRadius(float newRadius);
    float getRadius() const;

    const Vector3& getCenter() const;

private:
    float radius;
};

bool intersect(const CircleComponent& a, const CircleComponent& b);

#endif