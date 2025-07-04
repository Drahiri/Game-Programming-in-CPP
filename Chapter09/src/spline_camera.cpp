#include "spline_camera.h"

#include "actor.h"

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

SplineCamera::SplineCamera(Actor* owner) :
    CameraComponent(owner),
    index(1),
    t(0.0f),
    speed(0.5f),
    paused(true) {}

void SplineCamera::update(float deltaTime) {
    CameraComponent::update(deltaTime);

    // Update t value
    if(!paused) {
        t += speed * deltaTime;
        // Advance to the next control point if needed.
        // This assumes speed isn't so fast that you jump past
        // multiple control points on one frame
        if(t >= 1.0f) {
            // Make sure we have enough points to advance the path
            if(index < path.getNumPoints() - 3) {
                index++;
                t = t - 1.0f;
            } else {
                // Path's done, to pause
                paused = true;
            }
        }
    }

    // Camera position is the spline at the current t/index
    Vector3 cameraPos = path.compute(index, t);
    // Target point is juzst a small delta aread of the spline
    Vector3 target = path.compute(index, t + 0.01f);
    // Assume spline doesn't flip upside down
    const Vector3 up = Vector3::UnitZ;

    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    setViewMatrix(view);
}

void SplineCamera::setSpeed(float speed) {
    this->speed = speed;
}

void SplineCamera::setSpline(const Spline& spline) {
    path = spline;
}

void SplineCamera::setPaused(bool pause) {
    paused = pause;
}

void SplineCamera::restart() {
    index = 1;
    t = 0.0f;
    paused = false;
}