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
    void snapToIdeal();

    void setHorzDist(float dist);
    void setVertDist(float dist);
    void setTargetDist(float dist);
    void setSpringConstant(float spring);

    Component::TypeID getType() const override;

    void loadProperties(const rapidjson::Value& inObject) override;

private:
    // Horizontal follow distance
    float horzDist;
    // Vertical follow distance
    float vertDist;
    // TargetDistance
    float targetDist;

    // Spring constant (higher is stiffier);
    float springConstant;

    // Actual position of camera
    Vector3 actualPos;
    Vector3 velocity;
};

#endif