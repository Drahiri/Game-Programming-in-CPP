#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "component.h"
#include "math.h"

class Actor;
class Game;

class CameraComponent : public Component {
public:
    CameraComponent(Actor* owner);

    Component::TypeID getType() const override;

protected:
    void setViewMatrix(const Matrix4& view);
};

#endif
