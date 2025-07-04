#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

#include "camera_component.h"

class Actor;

class OrbitCamera : public CameraComponent {
public:
    OrbitCamera(Actor* owner);

    void update(float deltaTime) override;

    float getPitchSpeed() const;
    void setPitchSpeed(float speed);
    float getYawSpeed() const;
    void setYawSpeed(float speed);

private:
    // Offset from target
    Vector3 offset;
    // Up vector of camera
    Vector3 up;
    // Rotation/sec speed of pitch
    float pitchSpeed;
    // Rotation speed of yaw
    float yawSpeed;
};

#endif