#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "component.h"
#include "math.h"

class Actor;
class Game;

class CameraComponent : public Component {
public:
    CameraComponent(Actor* owner);

protected:
    void setViewMatrix(const Matrix4& view);
};

#endif
