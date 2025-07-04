#ifndef SPLINE_CAMERA_H
#define SPLINE_CAMERA_H

#include "camera_component.h"
#include "math.h"

#include <vector>

class Actor;

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

class SplineCamera : public CameraComponent {
public:
    SplineCamera(Actor* owner);

    void update(float deltaTime) override;

    void setSpeed(float speed);
    void setSpline(const Spline& spline);
    void setPaused(bool pause);

private:
    // Spline path camera follows
    Spline path;
    // Current control point index and t
    size_t index;
    float t;

    // Amount t changed/sec
    float speed;
    // Wherther to move the camera along the path
    bool paused;
};

#endif