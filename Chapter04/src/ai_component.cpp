#include "ai_component.h"

#include "ai_state.h"

#include <SDL3/SDL_log.h>

AIComponent::AIComponent(Actor* owner) : Component(owner) {}

void AIComponent::update(float deltaTime) {
    if(currentState) {
        currentState->update(deltaTime);
    }
}

void AIComponent::changeState(const std::string& name) {
    // First exit the current state
    if(currentState) {
        currentState->onExit();
    }

    // Try to find the new state from the map
    auto iter = stateMap.find(name);
    if(iter != stateMap.end()) {
        currentState = iter->second;
        // We're entering the new state
        currentState->onEnter();
    } else {
        SDL_Log("Could not find AIState %s in state map.", name.c_str());
        currentState = nullptr;
    }
}

void AIComponent::registerState(AIState* state) {
    stateMap.emplace(state->getName(), state);
}
