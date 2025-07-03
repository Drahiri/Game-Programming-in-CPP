#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include "camera_component.h"

class Actor;

class FPSCamera : public CameraComponent {
public:
    FPSCamera(Actor* owner);

    void update(float deltaTime) override;
};

#endif