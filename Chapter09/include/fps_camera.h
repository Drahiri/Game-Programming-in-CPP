#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include "camera_component.h"

class Actor;

class FPSCamera : public CameraComponent {
public:
    FPSCamera(Actor* owner);

    void update(float deltaTime) override;

    float getPitchSpeed() const;
    void setPitchSpeed(float speed);
    float getMaxPitch() const;
    void setMaxPitch(float pitch);
    float getPitch() const;

private:
    // Rotation/sec speed of pitch
    float pitchSpeed;
    // Maximum pitch deviation from forward
    float maxPitch;
    // Current pitch
    float pitch;
};

#endif