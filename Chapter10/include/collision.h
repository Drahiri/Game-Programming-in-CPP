#ifndef COLLISION_H
#define COLLISION_H

#include "math.h"

#include <vector>

struct LineSegment {
    LineSegment(const Vector3& start, const Vector3& end);

    Vector3 start;
    Vector3 end;

    Vector3 pointOnSegment(float t) const;

    float minDistSq(const Vector3& point) const;
    static float minDistSq(const LineSegment& s1, const LineSegment& s2);
};

struct Plane {
    Plane(const Vector3& normal, float d);
    // Construct plane from three points
    Plane(const Vector3& a, const Vector3& b, const Vector3& c);

    // Get the signed distance between the point and the plane
    float signedDist(const Vector3& point);

    Vector3 normal;
    float d;
};

struct Sphere {
    Sphere(const Vector3& center, float radius);

    bool contains(const Vector3& point) const;

    Vector3 center;
    float radius;
};

struct AABB {
    AABB(const Vector3& min, const Vector3& max);
    // Update min and max accounting for this point
    void updateMinMax(const Vector3& point);
    void rotate(const Quaternion& q);
    bool contains(const Vector3& point) const;
    float minDistSq(const Vector3& point) const;

    Vector3 min;
    Vector3 max;
};

struct OBB {
    Vector3 center;
    Quaternion rotation;
    Vector3 extents;
};

struct Capsule {
    Capsule(const Vector3& start, const Vector3& end, float radius);

    bool contains(const Vector3& point) const;

    LineSegment segment;
    float radius;
};

struct ConvexPolygon {
    bool contains(const Vector2& point) const;

    // Vertices have a clockwise ordering
    std::vector<Vector2> vertices;
};

bool intersect(const Sphere& a, const Sphere& b);
bool intersect(const AABB& a, const AABB& b);
bool intersect(const Sphere& s, const AABB& box);
bool intersect(const Capsule& a, const Capsule& b);
bool intersect(const LineSegment& l, const Plane& p, float& outT);
bool intersect(const LineSegment& l, const Sphere& s, float& outT);
bool intersect(const LineSegment& l, const AABB& b, float& outT);

bool sweptSphere(
      const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float& outT);

#endif