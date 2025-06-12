#include "ship.h"

#include "game.h"
#include "input_component.h"
#include "laser.h"
#include "sprite_component.h"

Ship::Ship(Game* game) : Actor(game), rightSpeed(0.0f), downSpeed(0.0f), laserCooldown(0.0f) {
    SpriteComponent* sc = new SpriteComponent(this);
    sc->setTexture(game->getTexture("assets/ShipWithThrust.png"));

    InputComponent* ic = new InputComponent(this);
    ic->setForwardKey(SDL_SCANCODE_W);
    ic->setBackKey(SDL_SCANCODE_S);
    ic->setClockwiseKey(SDL_SCANCODE_A);
    ic->setCounterClockwiseKey(SDL_SCANCODE_D);
    ic->setMaxForwardSpeed(300.0f);
    ic->setMaxAngularSpeed(Math::TwoPi);
}

void Ship::updateActor(float deltaTime) {
    laserCooldown -= deltaTime;
}

void Ship::actorInput(const bool* keyState) {
    if(keyState[SDL_SCANCODE_SPACE] && laserCooldown <= 0.0f) {
        // Create a laser and set it's position/rotation
        Laser* laser = new Laser(getGame());
        laser->setPosition(getPosition());
        laser->setRotation(getRotation());

        // Reset laser cooldown (half second)
        laserCooldown = 0.5f;
    }
}

float Ship::getRightSpeed() const {
    return rightSpeed;
}

float Ship::getDownSpeed() const {
    return downSpeed;
}
