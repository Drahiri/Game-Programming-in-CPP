#ifndef COLLISION_H
#define COLLISION_H

#include "math.h"

#include <vector>

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

struct Sphere {
    Vector3 center;
    float radius;
};

struct AABB {
    Vector3 min;
    Vector3 max;

    void updateMinMax(const Vector3& point);
    void rotate(const Quaternion& q);
};

struct OBB {
    Vector3 center;
    Quaternion rotation;
    Vector3 extents;
};

struct Capsule {
    LineSegment segment;
    float radius;
};

struct ConvexPolygon {
    // Vertices have a clockwise ordering
    std::vector<Vector2> vertices;
};
#endif