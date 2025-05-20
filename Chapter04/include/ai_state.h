#ifndef AI_STATE_H
#define AI_STATE_H

class AIComponent;

class AIState {
public:
    AIState(AIComponent* owner);

    // State-specific behavior
    virtual void update(float deltaTime) = 0;
    virtual void onEnter() = 0;
    virtual void onExit() = 0;

    // Getter for string name of state
    virtual const char* getName() const = 0;

protected:
    AIComponent* owner;
};

#endif