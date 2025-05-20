#include "ai_state.h"

#include "ai_component.h"

#include <SDL3/SDL_log.h>

AIState::AIState(AIComponent* owner) : owner(owner) {}

AIPatrol::AIPatrol(AIComponent* owner) : AIState(owner) {}

void AIPatrol::update(float deltaTime) {
    SDL_Log("Updating %s state", getName());
    bool dead = true;
    if(dead) {
        owner->changeState("Death");
    }
}

void AIPatrol::onEnter() {
    SDL_Log("Entering %s state", getName());
}

void AIPatrol::onExit() {
    SDL_Log("Exiting %s state", getName());
}

const char* AIPatrol::getName() const {
    return "Patrol";
}

AIDeath::AIDeath(AIComponent* owner) : AIState(owner) {}

void AIDeath::update(float deltaTime) {
    SDL_Log("Updating %s state", getName());
}

void AIDeath::onEnter() {
    SDL_Log("Entering %s state", getName());
}

void AIDeath::onExit() {
    SDL_Log("Exiting %s state", getName());
}

const char* AIDeath::getName() const {
    return "Death";
}

AIAttack::AIAttack(AIComponent* owner) : AIState(owner) {}

void AIAttack::update(float deltaTime) {
    SDL_Log("Updating %s state", getName());
}

void AIAttack::onEnter() {
    SDL_Log("Entering %s state", getName());
}

void AIAttack::onExit() {
    SDL_Log("Exiting %s state", getName());
}

const char* AIAttack::getName() const {
    return "Attack";
}
