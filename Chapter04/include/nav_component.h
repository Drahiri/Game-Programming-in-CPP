#ifndef NAV_COMPONENT_H
#define NAV_COMPONENT_H

#include "math.h"
#include "move_component.h"

class Actor;
class Tile;

class NavComponent : public MoveComponent {
public:
    NavComponent(Actor* owner, int updateOrder = 10);

    void update(float deltaTime) override;
    void startPath(const Tile* start);
    void turnTo(const Vector2& pos);

private:
    const Tile* nextPoint;
};

#endif