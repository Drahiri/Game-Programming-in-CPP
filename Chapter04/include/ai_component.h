#ifndef AI_COMPONENT_H
#define AI_COMPONENT_H

#include "component.h"

#include <string>
#include <unordered_map>

class Actor;
class AIState;

class AIComponent : public Component {
public:
    AIComponent(Actor* owner);

    void update(float deltaTime) override;
    void changeState(const std::string& name);

    // Add a new state to the map
    void registerState(AIState* state);

private:
    // Maps name of state to AIState instance
    std::unordered_map<std::string, AIState*> stateMap;

    // Current state we're in
    AIState* currentState;
};

#endif