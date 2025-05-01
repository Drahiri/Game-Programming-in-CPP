#include "actor.h"

#include "game.h"

Actor::Actor(Game* game) :
    state(State::Active),
    position(Vector2::Zero),
    scale(1.0f),
    rotation(0.0f),
    game(game) {}

Actor::~Actor() {}

void Actor::update(float deltaTime) {
    if(state != State::Active) {
        return;
    }

    updateComponents(deltaTime);
    updateActor(deltaTime);
}

void Actor::updateComponents(float deltaTime) {
    for(auto component: components) {
        // TODO: update components
    }
}

void Actor::updateActor(float deltaTime) {}

Actor::State Actor::getState() const {
    return state;
}

void Actor::setState(State newState) {
    state = newState;
}

const Vector2& Actor::getPosition() const {
    return position;
}

void Actor::setPosition(const Vector2& newPosition) {
    position = newPosition;
}

float Actor::getScale() const {
    return scale;
}

void Actor::setScale(float newScale) {
    scale = newScale;
}

float Actor::getRotation() const {
    return rotation;
}

void Actor::setRotation(float newRotation) {
    rotation = newRotation;
}

void Actor::addComponent(Component* component) {
    // TODO: adding components
}

void Actor::removeComponent(Component* components) {
    // TODO: Removing components
}
