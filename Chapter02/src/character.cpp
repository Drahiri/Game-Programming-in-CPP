#include "character.h"

#include "anim_sprite_component.h"
#include "game.h"

Character::Character(Game* game) : Actor(game), jumping(false), punching(false) {
    asc = new AnimSpriteComponent(this);
    std::vector<SDL_Texture*> anims = { game->getTexture("assets/Character01.png"),
        game->getTexture("assets/Character02.png"),
        game->getTexture("assets/Character03.png"),
        game->getTexture("assets/Character04.png"),
        game->getTexture("assets/Character05.png"),
        game->getTexture("assets/Character06.png") };

    asc->addAnimation("walking", anims);
    asc->setAnimFPS("walking", 6);
    asc->playAnimation("walking");

    anims = { game->getTexture("assets/Character07.png"),
        game->getTexture("assets/Character08.png"),
        game->getTexture("assets/Character09.png"),
        game->getTexture("assets/Character10.png"),
        game->getTexture("assets/Character11.png"),
        game->getTexture("assets/Character12.png"),
        game->getTexture("assets/Character13.png"),
        game->getTexture("assets/Character14.png"),
        game->getTexture("assets/Character15.png") };

    asc->addAnimation("jumping", anims);
    asc->setAnimFPS("jumping", 6);

    anims = { game->getTexture("assets/Character16.png"),
        game->getTexture("assets/Character17.png"),
        game->getTexture("assets/Character18.png") };

    asc->addAnimation("punching", anims);
    asc->setAnimFPS("punching", 6);
}

Character::~Character() {
    asc = nullptr;
}

void Character::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    if(jumping) {
        asc->playAnimation("jumping");
    } else if(punching) {
        asc->playAnimation("punching");
    } else {
        asc->playAnimation("walking");
    }
}

void Character::processKeyboard(const bool* state) {
    if(state[SDL_SCANCODE_W]) {
        jumping = true;
    }
    if(!state[SDL_SCANCODE_W]) {
        jumping = false;
    }
    if(state[SDL_SCANCODE_SPACE]) {
        punching = true;
    }
    if(!state[SDL_SCANCODE_SPACE]) {
        punching = false;
    }
}

bool Character::getJumping() const {
    return jumping;
}

bool Character::getPunching() const {
    return punching;
}
