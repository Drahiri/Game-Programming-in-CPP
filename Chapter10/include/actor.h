#ifndef ACTOR_H
#define ACTOR_H

#include "math.h"

#include <vector>

class Component;
class Game;

class Actor {
public:
    // Used to track state of actor
    enum class State {
        Active,
        Paused,
        Dead
    };

    // Constructor/destructor
    Actor(Game* game);
    virtual ~Actor();

    // Update function called from Game (not overridable)
    void update(float deltaTime);
    // Updates all the components attached to the actor
    void updateComponents(float deltaTime);
    // Any actor-specific update code (overridable)
    virtual void updateActor(float deltaTime);

    // Getters/setters
    State getState() const;
    void setState(State newState);

    const Vector3& getPosition() const;
    void setPosition(const Vector3& newPosition);

    float getScale() const;
    void setScale(float newScale);

    const Quaternion& getRotation() const;
    void setRotation(const Quaternion& newRotation);

    Vector3 getForward() const;
    Vector3 getRight() const;

    Game* getGame() const;

    // Add/remove components
    void addComponent(Component* component);
    void removeComponent(Component* component);

    // ProcessInput function called from Game (not overridable)
    void processInput(const bool* keyState);

    // Any actor-specific input code (overridable)
    virtual void actorInput(const bool* keyState);

    // Matrix transforms
    void computeWorldTransform();
    const Matrix4& getWorldTransform() const;

    void rotateToNewForward(const Vector3& forward);

private:
    // Actor's state
    State state;

    // Transform
    Vector3 position;    // Center position of actor
    float scale;         // Uniforms scale of actor (1.0f)
    Quaternion rotation; // Rotation quaternion

    Matrix4 worldTransform;
    bool recomputeWorldTransform;

    // Components held by this actor
    std::vector<Component*> components;
    Game* game;
};

#endif