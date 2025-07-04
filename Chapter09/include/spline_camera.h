#ifndef SPLINE_CAMERA_H
#define SPLINE_CAMERA_H

#include "math.h"

#include <vector>

struct Spline {
    // Control points for spline
    // (Requires n + 2 points where n is number)
    // of points in segment)
    std::vector<Vector3> controlPoints;

    // Given spline segment where startIdx = P1,
    // compute position based on t value
    Vector3 compute(size_t startIdx, float t) const;

    size_t getNumPoints() const;
};

#endif