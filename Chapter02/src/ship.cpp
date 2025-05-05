#include "ship.h"

#include "anim_sprite_component.h"
#include "game.h"

Ship::Ship(Game* game) : Actor(game), rightSpeed(0.0f), downSpeed(0.0f) {
    AnimSpriteComponent* asc = new AnimSpriteComponent(this);
    std::vector<SDL_Texture*> anims = { game->getTexture("assets/Ship01.png"),
        game->getTexture("assets/Ship02.png"),
        game->getTexture("assets/Ship03.png"),
        game->getTexture("assets/Ship04.png") };

    asc->addAnimation(anims);
}

void Ship::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    // Update position based on speeds and delta time
    Vector2 pos = getPosition();

    pos.x += rightSpeed * deltaTime;
    pos.y += downSpeed * deltaTime;

    // Restrict position to left half of screen
    if(pos.x < 0) {
        pos.x = 0;
    } else if(pos.x > getGame()->getScreenSize().x / 2.0f) {
        pos.x = getGame()->getScreenSize().x / 2.0f;
    }

    if(pos.y < 0) {
        pos.y = 0;
    } else if(pos.y > getGame()->getScreenSize().y) {
        pos.y = getGame()->getScreenSize().y;
    }

    setPosition(pos);
}

void Ship::processKeyboard(const bool* state) {
    rightSpeed = 0.0f;
    downSpeed = 0.0f;

    // right/left
    if(state[SDL_SCANCODE_D]) {
        rightSpeed += 250.0f;
    }
    if(state[SDL_SCANCODE_A]) {
        rightSpeed -= 250.0f;
    }

    // up/down
    if(state[SDL_SCANCODE_S]) {
        downSpeed += 300.0f;
    }
    if(state[SDL_SCANCODE_W]) {
        downSpeed -= 250.0f;
    }
}

float Ship::getRightSpeed() const {
    return rightSpeed;
}

float Ship::getDownSpeed() const {
    return downSpeed;
}
