#include "laser.h"

#include "asteroid.h"
#include "circle_component.h"
#include "game.h"
#include "move_component.h"
#include "sprite_component.h"

Laser::Laser(Game* game) : Actor(game), deathTimer(1.0f) {
    // Create a sprite component
    SpriteComponent* sc = new SpriteComponent(this);
    sc->setTexture(game->getTexture("assets/Laser.png"));

    // Create a move component, and set a forward speed
    MoveComponent* mc = new MoveComponent(this);
    mc->setForwardSpeed(800.0f);

    // Create a circle component (for collision)
    circle = new CircleComponent(this);
    circle->setRadius(11.0f);
}

void Laser::updateActor(float deltaTime) {
    // If we run out of time, laser is dead
    deathTimer -= deltaTime;
    if(deathTimer <= 0.0f) {
        setState(State::Dead);
    } else {
        // Do we intersect with an asteroid?
        for(auto ast: getGame()->getAsteroids()) {
            if(intersect(*circle, *(ast->getCircle()))) {
                // If this laser intersects with a asteroid
                // set ourselves and the asteroid to dead
                setState(State::Dead);
                ast->setState(State::Dead);
                break;
            }
        }
    }
}