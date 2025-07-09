#ifndef COLLISION_H
#define COLLISION_H

#include "math.h"

struct LineSegment {
    Vector3 start;
    Vector3 end;

    Vector3 pointOnSegment(float t) const;

    float minDistSq(const Vector3& point) const;
};

#endif