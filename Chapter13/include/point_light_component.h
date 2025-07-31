#ifndef POINT_LIGHT_COMPONENT_H
#define POINT_LIGHT_COMPONENT_H

#include "component.h"
#include "math.h"

class Actor;
class Mesh;
class Shader;

class PointLightComponent : public Component {
public:
    PointLightComponent(Actor* owner);
    ~PointLightComponent();

    // Draw this point light as geometry
    void draw(Shader* shader, Mesh* mesh);

    // Diffuse color
    Vector3 diffuseColor;
    // Radius of light
    float innerRadius;
    float outerRadius;
};

#endif