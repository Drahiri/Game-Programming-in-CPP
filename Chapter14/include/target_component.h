#ifndef TARGET_COMPONENT_H
#define TARGET_COMPONENT_H

#include "component.h"

class Actor;

class TargetComponent : public Component {
public:
    TargetComponent(Actor* owner);
    ~TargetComponent();
};

#endif