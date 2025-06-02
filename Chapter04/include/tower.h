#ifndef TOWER_H
#define TOWER_H

#include "actor.h"
#include "ai_component.h"
#include "ai_state.h"

class Game;
class MoveComponent;
class Enemy;

class Tower : public Actor {
public:
    Tower(Game* game);
    void updateActor(float deltaTime) override;

private:
    MoveComponent* move;
    float nextAttack;
    const float attackTime = 2.5f;
    const float attackRange = 100.0f;
};

class TowerAIComponent : public AIComponent {
public:
    TowerAIComponent(Actor* owner);
    Enemy* getTarget() const;
    void setTarget(Enemy* enemy);

private:
    Enemy* target;
};

class TowerPatrolState : public AIState {
public:
    TowerPatrolState(TowerAIComponent* owner);

    void onEnter() override {}

    void onExit() override {}

    void update(float deltaTime) override;
    const char* getName() const override;

private:
    Actor* owningActor;
    TowerAIComponent* owningComponent;
    const float attackRange = 100.0f;
};

class TowerShootState : public AIState {
public:
    TowerShootState(TowerAIComponent* owner);

    void onEnter() override;

    void onExit() override;

    void update(float deltaTime) override;
    const char* getName() const override;

private:
    TowerAIComponent* owningComponent;
};

class TowerReloadState : public AIState {
public:
    TowerReloadState(TowerAIComponent* owner);

    void onEnter() override;

    void onExit() override {}

    void update(float deltaTime) override;
    const char* getName() const override;

private:
    float nextAttack;
    const float reloadTime = 2.5f;
};

#endif