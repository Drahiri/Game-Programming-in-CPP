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

class AIPatrol : public AIState {
public:
    AIPatrol(AIComponent* owner);

    // Override the behaviors for this state
    void update(float deltaTime) override;
    void onEnter() override;
    void onExit() override;

    const char* getName() const override;
};

class AIDeath : public AIState {
public:
    AIDeath(AIComponent* owner);

    // Override the behaviors for this state
    void update(float deltaTime) override;
    void onEnter() override;
    void onExit() override;

    const char* getName() const override;
};

class AIAttack : public AIState {
public:
    AIAttack(AIComponent* owner);

    // Override the behaviors for this state
    void update(float deltaTime) override;
    void onEnter() override;
    void onExit() override;

    const char* getName() const override;
};

#endif