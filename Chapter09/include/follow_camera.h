#ifndef FOLLOW_CAMERA_H
#define FOLLOW_CAMERA_H

#include "camera_component.h"
#include "math.h"

class Actor;

class FollowCamera : public CameraComponent {
public:
    FollowCamera(Actor* owner);

    void update(float deltaTime) override;

    Vector3 computeCameraPos() const;

    void setHorzDist(float dist);
    void setVertDist(float dist);
    void setTargetDist(float dist);

private:
    // Horizontal follow distance
    float horzDist;
    // Vertical follow distance
    float vertDist;
    // TargetDistance
    float targetDist;
};

#endif