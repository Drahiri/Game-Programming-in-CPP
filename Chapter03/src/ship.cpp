#include "ship.h"

#include "anim_sprite_component.h"
#include "game.h"
#include "input_component.h"

Ship::Ship(Game* game) : Actor(game), rightSpeed(0.0f), downSpeed(0.0f) {
    AnimSpriteComponent* asc = new AnimSpriteComponent(this);
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
}

void Ship::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    // Update position based on speeds and delta time
    Vector2 pos = getPosition();

    pos.x += rightSpeed * deltaTime;
    pos.y += downSpeed * deltaTime;

    setPosition(pos);
}

float Ship::getRightSpeed() const {
    return rightSpeed;
}

float Ship::getDownSpeed() const {
    return downSpeed;
}
