#include "ship.h"

#include "game.h"
#include "input_system.h"
#include "laser.h"
#include "sprite_component.h"

Ship::Ship(Game* game) :
    Actor(game),
    rightSpeed(0.0f),
    downSpeed(0.0f),
    laserCooldown(0.0f),
    velocitDir(Vector2::Zero),
    rotationDir(Vector2::Zero),
    speed(300.0f) {
    SpriteComponent* sc = new SpriteComponent(this);
    sc->setTexture(game->getTexture("assets/ShipWithThrust.png"));
}

void Ship::updateActor(float deltaTime) {
    laserCooldown -= deltaTime;

    Vector2 pos = getPosition();
    pos += velocitDir * speed * deltaTime;
    setPosition(pos);

    float angle = Math::Atan2(rotationDir.y, rotationDir.x);
    setRotation(angle);
}

void Ship::actorInput(const InputState& inState) {
    if(inState.gamepads[0].getIsConnected()) {
        velocitDir = inState.gamepads[0].getLeftStick();
        if(!Math::NearZero(inState.gamepads[0].getRightStick().Length())) {
            rotationDir = inState.gamepads[0].getRightStick();
        }
    }

    if(inState.gamepads[0].getRightTrigger() > 0.25f && laserCooldown <= 0.0f) {
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
