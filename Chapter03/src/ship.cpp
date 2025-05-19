#include "ship.h"

#include "anim_sprite_component.h"
#include "asteroid.h"
#include "circle_component.h"
#include "game.h"
#include "input_component.h"
#include "laser.h"

Ship::Ship(Game* game) :
    Actor(game),
    rightSpeed(0.0f),
    downSpeed(0.0f),
    laserCooldown(0.0f),
    invisibleTime(0.0f) {
    asc = new AnimSpriteComponent(this);
    std::vector<SDL_Texture*> anims = { game->getTexture("assets/Ship01.png"),
        game->getTexture("assets/Ship02.png"),
        game->getTexture("assets/Ship03.png"),
        game->getTexture("assets/Ship04.png") };

    asc->setAnimTextures(anims);
    InputComponent* ic = new InputComponent(this);
    ic->setForwardKey(SDL_SCANCODE_W);
    ic->setBackKey(SDL_SCANCODE_S);
    ic->setClockwiseKey(SDL_SCANCODE_A);
    ic->setCounterClockwiseKey(SDL_SCANCODE_D);
    ic->setMaxForwardSpeed(300.0f);
    ic->setMaxAngularSpeed(Math::TwoPi);

    circle = new CircleComponent(this);
    circle->setRadius(15.0f);
}

void Ship::updateActor(float deltaTime) {
    laserCooldown -= deltaTime;
    invisibleTime -= deltaTime;

    if(!asc->getVisible()) {
        if(invisibleTime > 0.0f) {
            return;
        } else {
            asc->setVisible(true);
        }
    }

    for(auto ast: getGame()->getAsteroids()) {
        if(intersect(*circle, *(ast->getCircle()))) {
            // If this ship intersects with a asteroid
            // set ship position to center and destroy asteroid
            ast->setState(State::Dead);

            Vector2 screenSize = getGame()->getScreenSize();

            setPosition(Vector2{ screenSize.x / 2.0f, screenSize.y / 2.0f });
            setRotation(0);
            asc->setVisible(false);
            invisibleTime = 1.0f;
            break;
        }
    }
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
