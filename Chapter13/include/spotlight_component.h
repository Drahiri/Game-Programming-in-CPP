#ifndef SPOTLIGHT_COMPONENT_H
#define SPOTLIGHT_COMPONENT_H

#include "component.h"
#include "math.h"

class Actor;
class Mesh;
class Shader;

class SpotlightComponent : public Component {
public:
    SpotlightComponent(Actor* owner);
    ~SpotlightComponent();

    void draw(Shader* shader, Mesh* mesh);

    Vector3 diffuseColor;
    float innerRadius;
    float outerRadius;
    float angle;

    const float DEFAULT_HALF_ANGLE = 30.0f;
};

#endif