#ifndef MOVE_COMPONENT_H
#define MOVE_COMPONENT_H

#include "component.h"
#include "math.h"

class MoveComponent : public Component {
public:
    // Lower update order to update first
    MoveComponent(Actor* owner, int updateOrder = 10);

    void update(float deltaTime) override;

    float getAngularSpeed() const;
    void setAngularSpeed(float speed);

    float getForwardSpeed() const;
    void setForwardSpeed(float speed);

    float getStrafeSpeed() const;
    void setStrafeSpeed(float speed);

    Component::TypeID getType() const override;

    void loadProperties(const rapidjson::Value& inObject) override;

private:
    // Controls rotation (radians/second)
    float angularSpeed;
    // Controls forward movement (units/second)
    float forwardSpeed;
    // Controls strafe speed
    float strafeSpeed;

    // Screen size
    Vector2 screenSize;
};

#endif