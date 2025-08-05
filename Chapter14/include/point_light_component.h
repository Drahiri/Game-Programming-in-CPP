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

    Component::TypeID getType() const;

    void loadProperties(const rapidjson::Value& inObject) override;
    void saveProperties(
          rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject) const override;

    // Diffuse color
    Vector3 diffuseColor;
    // Radius of light
    float innerRadius;
    float outerRadius;
};

#endif