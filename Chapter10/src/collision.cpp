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

bool Sphere::contains(const Vector3& point) const {
    // Get distance squared between center and point
    float distSq = (center - point).LengthSq();
    return distSq <= (radius * radius);
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

bool AABB::contains(const Vector3& point) const {
    bool outside = point.x < min.x || point.y < min.y || point.z < min.z || point.x > max.x
                   || point.y > max.y || point.z > max.z;

    // If none of these are true, the point is inside box
    return !outside;
}

float AABB::minDistSq(const Vector3& point) const {
    // Compute differences for each axis
    float dx = Math::Max(min.x - point.x, 0.0f);
    dx = Math::Max(dx, point.x - max.x);
    float dy = Math::Max(min.y - point.y, 0.0f);
    dy = Math::Max(dy, point.y - max.y);
    float dz = Math::Max(min.z - point.z, 0.0f);
    dz = Math::Max(dz, point.z - max.z);

    // Distance squared formula
    return dx * dx + dy * dy + dz * dz;
}

bool Capsule::contains(const Vector3& point) const {
    // Get minimal dist. sq. between point and line segment
    float distSq = segment.minDistSq(point);
    return distSq <= (radius * radius);
}

bool ConvexPolygon::contains(const Vector2& point) const {
    float sum = 0.0f;
    Vector2 a, b;

    for(size_t i = 0; i < vertices.size() - 1; i++) {
        // From point to first vertex
        a = vertices[i] - point;
        a.Normalize();
        // From point to second vertex
        b = vertices[i + 1] - point;
        b.Normalize();
        // Add angle to sum
        sum += Math::Acos(Vector2::Dot(a, b));
    }

    // Compute angle for last vertex and first vertex
    a = vertices.back() - point;
    a.Normalize();
    b = vertices[0] - point;
    b.Normalize();
    sum += Math::Acos(Vector2::Dot(a, b));

    // Return true if approximately 2pi
    return Math::NearZero(sum - Math::TwoPi);
}

bool intersect(const Sphere& a, const Sphere& b) {
    float distSq = (a.center - b.center).LengthSq();
    float sumRadii = a.radius + b.radius;
    return distSq < (sumRadii * sumRadii);
}

bool intersect(const AABB& a, const AABB& b) {
    bool no = a.max.x < b.min.x || a.max.y < b.min.y || a.max.z < b.min.z || b.max.x < a.min.x
              || b.max.y < a.min.y || b.max.z < a.min.z;

    // If none of these are true, they must intersect
    return !no;
}

bool intersect(const Sphere& s, const AABB& box) {
    float distSq = box.minDistSq(s.center);
    return distSq <= (s.radius * s.radius);
}