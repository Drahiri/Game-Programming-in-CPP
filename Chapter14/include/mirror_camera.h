#ifndef MIRROR_CAMERA_H
#define MIRROR_CAMERA_H

#include "camera_component.h"

class Actor;

class MirrorCamera : public CameraComponent {
public:
    MirrorCamera(Actor* owner);

    void update(float deltaTime) override;

    void snapToIdeal();

    void setHorzDist(float dist);
    void setVertDist(float dist);
    void setTargetDist(float dist);

    Component::TypeID getType() const override;

private:
    Vector3 computeCameraPos() const;

    float horzDist;
    float vertDist;
    float targetDist;
};

#endif