#include "actor.h"

#include "component.h"
#include "game.h"

#include <algorithm>

Actor::Actor(Game* game) :
    state(State::Active),
    position(Vector2::Zero),
    scale(1.0f),
    rotation(0.0f),
    recomputeWorldTransform(true),
    game(game) {
    this->game->addActor(this);
}

Actor::~Actor() {
    game->removeActor(this);
    // Because ~Component call removeComponent, need a different style loop
    while(!components.empty()) {
        delete components.back();
    }
}

void Actor::update(float deltaTime) {
    if(state != State::Active) {
        return;
    }

    computeWorldTransform();
    updateComponents(deltaTime);
    updateActor(deltaTime);
    computeWorldTransform();
}

void Actor::updateComponents(float deltaTime) {
    for(auto component: components) {
        component->update(deltaTime);
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
    recomputeWorldTransform = true;
}

float Actor::getScale() const {
    return scale;
}

void Actor::setScale(float newScale) {
    scale = newScale;
    recomputeWorldTransform = true;
}

float Actor::getRotation() const {
    return rotation;
}

void Actor::setRotation(float newRotation) {
    rotation = newRotation;
    recomputeWorldTransform = true;
}

Game* Actor::getGame() const {
    return game;
}

Vector2 Actor::getForward() const {
    return Vector2(Math::Cos(rotation), -Math::Sin(rotation));
}

void Actor::processInput(const bool* keyState) {
    if(state == State::Active) {
        for(auto comp: components) {
            comp->processInput(keyState);
        }
        actorInput(keyState);
    }
}

void Actor::actorInput(const bool* keyState) {}

void Actor::addComponent(Component* component) {
    // Find the insertion point in the sorted vector
    // (The first element with a order higher than me)
    int componentOrder = component->getUpdateOrder();
    auto iter = components.begin();

    for(; iter != components.end(); iter++) {
        if(componentOrder < (*iter)->getUpdateOrder()) {
            break;
        }
    }

    components.insert(iter, component);
}

void Actor::removeComponent(Component* component) {
    auto iter = std::find(components.begin(), components.end(), component);
    if(iter != components.end()) {
        components.erase(iter);
    }
}

void Actor::computeWorldTransform() {
    if(recomputeWorldTransform) {
        recomputeWorldTransform = false;

        // Scale, then rotate, then translate
        worldTransform = Matrix4::CreateScale(scale);
        worldTransform *= Matrix4::CreateRotationZ(rotation);
        worldTransform *= Matrix4::CreateTranslation(Vector3(position.x, position.y, 0.0f));

        // Inform components world transform updated
        for(auto comp: components) {
            comp->onUpdateWorldTransform();
        }
    }
}

const Matrix4& Actor::getWorldTransform() const {
    return worldTransform;
}