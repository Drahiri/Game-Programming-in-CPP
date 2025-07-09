#include "collision.h"

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
