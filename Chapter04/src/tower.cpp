#include "tower.h"

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
}

void Tower::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    nextAttack -= deltaTime;

    if(nextAttack <= 0.0f) {
        Enemy* e = getGame()->getNearestEnemy(getPosition());
        if(e != nullptr) {
            // Vector from me to enemy
            Vector2 dir = e->getPosition() - getPosition();
            float dist = dir.Length();
            if(dist < attackRange) {
                // Rotate to face enemy
                setRotation(Math::Atan2(-dir.y, dir.x));
                // Spawn bullet at tower position facing enemy
                Bullet* b = new Bullet(getGame());
                b->setPosition(getPosition());
                b->setRotation(getRotation());
            }
        }
        nextAttack += attackTime;
    }
}