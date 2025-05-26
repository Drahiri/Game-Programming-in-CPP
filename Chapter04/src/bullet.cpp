#include "bullet.h"

#include "circle_component.h"
#include "enemy.h"
#include "game.h"
#include "move_component.h"
#include "sprite_component.h"

Bullet::Bullet(Game* game) : Actor(game) {
    SpriteComponent* sc = new SpriteComponent(this);
    sc->setTexture(getGame()->getTexture("assets/Projectile.png"));

    MoveComponent* mc = new MoveComponent(this);
    mc->setForwardSpeed(400.0f);

    circle = new CircleComponent(this);
    circle->setRadius(5.0f);

    liveTime = 1.0f;
}

void Bullet::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    // Check for collision vs enemies
    for(Enemy* e: getGame()->getEnemies()) {
        if(intersect(*circle, *(e->getCircle()))) {
            // We both die on collision
            e->setState(State::Dead);
            setState(State::Dead);
            break;
        }
    }

    liveTime -= deltaTime;
    if(liveTime <= 0.0f) {
        // Time limit hit, die
        setState(State::Dead);
    }
}