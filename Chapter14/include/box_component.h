#ifndef BOX_COMPONENT_H
#define BOX_COMPONENT_H

#include "collision.h"
#include "component.h"

#include <rapidjson/document.h>

class Actor;

class BoxComponent : public Component {
public:
    BoxComponent(Actor* owner);
    ~BoxComponent();

    void onUpdateWorldTransform() override;
    void setObjectBox(const AABB& model);
    const AABB& getWorldBox() const;
    void setShouldRotate(bool value);

    Component::TypeID getType() const override;

    void loadProperties(const rapidjson::Value& inObject) override;
    void saveProperties(
          rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject) const override;

private:
    AABB objectBox;
    AABB worldBox;
    bool shouldRotate;
};

#endif