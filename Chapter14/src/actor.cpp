#include "actor.h"

#include "component.h"
#include "game.h"
#include "level_loader.h"

#include <algorithm>

const char* Actor::typeNames[static_cast<int>(Actor::TypeID::NUM_ACTOR_TYPES)]{
    "Actor", "BallActor", "FollowActor", "PlaneActor", "TargetActor"
};

Actor::Actor(Game* game) :
    state(State::Active),
    position(Vector3::Zero),
    scale(1.0f),
    rotation(Quaternion::Identity),
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

const Vector3& Actor::getPosition() const {
    return position;
}

void Actor::setPosition(const Vector3& newPosition) {
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

const Quaternion& Actor::getRotation() const {
    return rotation;
}

void Actor::setRotation(const Quaternion& newRotation) {
    rotation = newRotation;
    recomputeWorldTransform = true;
}

Game* Actor::getGame() const {
    return game;
}

Vector3 Actor::getForward() const {
    return Vector3::Transform(Vector3::UnitX, rotation);
}

Vector3 Actor::getRight() const {
    // Rotate right axis using quaternion rotation
    return Vector3::Transform(Vector3::UnitY, rotation);
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
        worldTransform *= Matrix4::CreateFromQuaternion(rotation);
        worldTransform *= Matrix4::CreateTranslation(position);

        // Inform components world transform updated
        for(auto comp: components) {
            comp->onUpdateWorldTransform();
        }
    }
}

const Matrix4& Actor::getWorldTransform() const {
    return worldTransform;
}

void Actor::rotateToNewForward(const Vector3& forward) {
    // Figure out differences between original (unit x) and new
    float dot = Vector3::Dot(Vector3::UnitX, forward);
    float angle = Math::Acos(dot);

    // Are we facing down X?
    if(dot > 0.9999f) {
        setRotation(Quaternion::Identity);
    }
    // Are we facing down -X?
    else if(dot < -0.9999f) {
        setRotation(Quaternion(Vector3::UnitZ, Math::Pi));
    } else {
        // Rotate about axis from cross product
        Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
        axis.Normalize();
        setRotation(Quaternion(axis, angle));
    }
}

void Actor::loadProperties(const rapidjson::Value& inObj) {
    // Use strings for different states
    std::string state;
    if(JsonHelper::getString(inObj, "state", state)) {
        if(state == "active") {
            setState(State::Active);
        } else if(state == "paused") {
            setState(State::Paused);
        } else if(state == "dead") {
            setState(State::Dead);
        }
    }

    // Load position, rotation and scale, and compute transform
    JsonHelper::getVector3(inObj, "position", position);
    JsonHelper::getQuaternion(inObj, "rotation", rotation);
    JsonHelper::getFloat(inObj, "scale", scale);
    computeWorldTransform();
}

Component* Actor::getComponentOfType(Component::TypeID type) {
    Component* comp = nullptr;

    for(Component* c: components) {
        if(c->getType() == type) {
            comp = c;
            break;
        }
    }

    return comp;
}

Actor::TypeID Actor::getType() const {
    return TypeID::Actor;
}