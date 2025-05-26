#include "enemy.h"

#include "circle_component.h"
#include "game.h"
#include "grid.h"
#include "nav_component.h"
#include "sprite_component.h"
#include "tile.h"

#include <algorithm>

Enemy::Enemy(Game* game) : Actor(game) {
    // Add to enemy vector
    game->getEnemies().emplace_back(this);

    SpriteComponent* sc = new SpriteComponent(this);
    sc->setTexture(game->getTexture("assets/Airplane.png"));
    // Set position at start tile
    setPosition(getGame()->getGrid()->getStartTile()->getPosition());
    // Setup a nav component at the start tile
    NavComponent* nc = new NavComponent(this);
    nc->setForwardSpeed(150.0f);
    nc->startPath(getGame()->getGrid()->getStartTile());
    // Setup circle for collision
    circle = new CircleComponent(this);
    circle->setRadius(25.0f);
}

Enemy::~Enemy() {
    // Remove from enemy vector
    auto iter = std::find(getGame()->getEnemies().begin(), getGame()->getEnemies().end(), this);
    getGame()->getEnemies().erase(iter);
}

void Enemy::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    // Am I near the end tile?
    Vector2 diff = getPosition() - getGame()->getGrid()->getEndTile()->getPosition();
    if(Math::NearZero(diff.Length(), 10.0f)) {
        setState(State::Dead);
    }
}

CircleComponent* Enemy::getCircle() {
    return circle;
}
