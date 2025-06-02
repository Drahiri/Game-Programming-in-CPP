#include "tower.h"

#include "ai_component.h"
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "move_component.h"
#include "sprite_component.h"

Tower::Tower(Game* game) : Actor(game) {
    SpriteComponent* sc = new SpriteComponent(this);
    sc->setTexture(getGame()->getTexture("assets/Tower.png"));

    move = new MoveComponent(this);
    nextAttack = attackTime;

    TowerAIComponent* taic = new TowerAIComponent(this);
    taic->registerState(new TowerPatrolState(taic));
    taic->registerState(new TowerShootState(taic));
    taic->registerState(new TowerReloadState(taic));

    taic->changeState("Reload");
}

void Tower::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    // nextAttack -= deltaTime;

    // if(nextAttack <= 0.0f) {
    //     Enemy* e = getGame()->getNearestEnemy(getPosition());
    //     if(e != nullptr) {
    //         // Vector from me to enemy
    //         Vector2 dir = e->getPosition() - getPosition();
    //         float dist = dir.Length();
    //         if(dist < attackRange) {
    //             // Rotate to face enemy
    //             setRotation(Math::Atan2(-dir.y, dir.x));
    //             // Spawn bullet at tower position facing enemy
    //             Bullet* b = new Bullet(getGame());
    //             b->setPosition(getPosition());
    //             b->setRotation(getRotation());
    //         }
    //     }
    //     nextAttack += attackTime;
    // }
}

Enemy* TowerAIComponent::getTarget() const {
    return target;
}

void TowerAIComponent::setTarget(Enemy* enemy) {
    target = enemy;
}

TowerAIComponent::TowerAIComponent(Actor* owner) : AIComponent(owner) {}

TowerPatrolState::TowerPatrolState(TowerAIComponent* owner) :
    AIState(owner),
    owningComponent(owner) {
    owningActor = owner->getOwner();
}

void TowerPatrolState::update(float deltaTime) {
    Enemy* e = owningActor->getGame()->getNearestEnemy(owningActor->getPosition());
    if(e != nullptr) {
        Vector2 dir = e->getPosition() - owningActor->getPosition();
        float dist = dir.Length();
        if(dist < attackRange) {
            owningComponent->setTarget(e);
            owner->changeState("Shoot");
        }
    }
}

const char* TowerPatrolState::getName() const {
    return "Patrol";
}

TowerShootState::TowerShootState(TowerAIComponent* owner) :
    AIState(owner),
    owningComponent(owner) {}

void TowerShootState::onEnter() {
    Vector2 dir = owningComponent->getTarget()->getPosition() - owner->getOwner()->getPosition();
    owner->getOwner()->setRotation(Math::Atan2(-dir.y, dir.x));

    Bullet* b = new Bullet(owner->getOwner()->getGame());
    b->setPosition(owner->getOwner()->getPosition());
    b->setRotation(owner->getOwner()->getRotation());
}

void TowerShootState::update(float deltaTime) {
    owner->changeState("Reload");
}

void TowerShootState::onExit() {
    owningComponent->setTarget(nullptr);
}

const char* TowerShootState::getName() const {
    return "Shoot";
}

TowerReloadState::TowerReloadState(TowerAIComponent* owner) : AIState(owner), nextAttack(0.0f) {}

void TowerReloadState::onEnter() {
    nextAttack += reloadTime;
}

void TowerReloadState::update(float deltaTime) {
    nextAttack -= deltaTime;
    if(nextAttack <= 0.0f) {
        owner->changeState("Patrol");
    }
}

const char* TowerReloadState::getName() const {
    return "Reload";
}
