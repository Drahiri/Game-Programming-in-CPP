#include "spline_camera.h"

Vector3 Spline::compute(size_t startIdx, float t) const {
    // Check if startIdx is out of bounds
    if(startIdx >= controlPoints.size()) {
        return controlPoints.back();
    } else if(startIdx == 0) {
        return controlPoints[startIdx];
    } else if(startIdx + 2 >= controlPoints.size()) {
        return controlPoints[startIdx];
    }

    // Get p0 through p3
    Vector3 p0 = controlPoints[startIdx - 1];
    Vector3 p1 = controlPoints[startIdx];
    Vector3 p2 = controlPoints[startIdx + 1];
    Vector3 p3 = controlPoints[startIdx + 2];

    // Compute position accordin to Catmull-Rom equation
    // clang-format off
    Vector3 position = 0.5f * ((2.0f * p1) + (-1.0f * p0 + p2 ) * t + 
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t +
        (-1.0f * p0 + 3.0f * p1 - 3.0f * p2 + p3) * t * t * t);
    // clang-format on
    return position;
}

size_t Spline::getNumPoints() const {
    return controlPoints.size();
}