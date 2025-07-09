#include "collision.h"

#include <array>

Vector3 LineSegment::pointOnSegment(float t) const {
    return start + (end - start) * t;
}

float LineSegment::minDistSq(const Vector3& point) const {
    // Construct vectors
    Vector3 ab = end - start;
    Vector3 ba = -1.0f * ab;
    Vector3 ac = point - start;
    Vector3 bc = point - end;

    // Case 1: C projects prior to A
    if(Vector3::Dot(ab, ac) < 0.0f) {
        return ac.LengthSq();
    }
    // Case 2: C projects after B
    else if(Vector3::Dot(ba, bc) < 0.0f) {
        return bc.LengthSq();
    }
    // Case 3: C projects onto line
    else {
        // Compute p
        float scalar = Vector3::Dot(ac, ab) / Vector3::Dot(ab, ab);
        Vector3 p = scalar * ab;

        // Compute length squared of ac - p
        return (ac - p).LengthSq();
    }
}

Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c) {
    // Compute vectors from a to b, adn a to c
    Vector3 ab = b - a;
    Vector3 ac = c - a;

    // Cross product and normalize to get normal
    normal = Vector3::Cross(ab, ac);
    normal.Normalize();

    // d = -P dot n
    d = -Vector3::Dot(a, normal);
}

float Plane::signedDist(const Vector3& point) {
    return Vector3::Dot(point, normal) - d;
}

void AABB::updateMinMax(const Vector3& point) {
    // Update each component separately
    min.x = Math::Min(min.x, point.x);
    min.y = Math::Min(min.y, point.y);
    min.z = Math::Min(min.z, point.z);
    max.x = Math::Max(max.x, point.x);
    max.y = Math::Max(max.y, point.y);
    max.z = Math::Max(max.z, point.z);
}

void AABB::rotate(const Quaternion& q) {
    // Construct the 8 points for the cornders of the box
    std::array<Vector3, 8> points;
    // Min points is always a cornder
    points[0] = min;
    // Permutations with 2 min and 1 max
    points[1] = Vector3(max.x, min.y, min.z);
    points[2] = Vector3(min.x, max.y, min.z);
    points[3] = Vector3(min.x, min.y, max.z);
    // Permutations with 2 max and 1 min
    points[4] = Vector3(min.x, max.y, max.z);
    points[5] = Vector3(max.x, min.y, max.z);
    points[6] = Vector3(max.x, max.y, min.z);

    // Rotate first point
    Vector3 p = Vector3::Transform(points[0], q);
    // Reset min/max to first point rotatd
    min = p;
    max = p;
    // Update min/max based on remaining points, rotated
    for(size_t i = 1; i < points.size(); i++) {
        p = Vector3::Transform(points[i], q);
        updateMinMax(p);
    }
}
