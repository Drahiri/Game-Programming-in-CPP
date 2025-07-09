#ifndef COLLISION_H
#define COLLISION_H

#include "math.h"

struct LineSegment {
    Vector3 start;
    Vector3 end;

    Vector3 pointOnSegment(float t) const;

    float minDistSq(const Vector3& point) const;
};

struct Plane {
    Vector3 normal;
    float d;

    Plane(const Vector3& a, const Vector3& b, const Vector3& c);
    float signedDist(const Vector3& point);
};

#endif