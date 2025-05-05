#include "character.h"

#include "anim_sprite_component.h"
#include "game.h"

Character::Character(Game* game) : Actor(game), jumping(false), punching(false) {
    AnimSpriteComponent* asc = new AnimSpriteComponent(this);
    std::vector<SDL_Texture*> anims = { game->getTexture("assets/Character01.png"),
        game->getTexture("assets/Character02.png"),
        game->getTexture("assets/Character03.png"),
        game->getTexture("assets/Character04.png"),
        game->getTexture("assets/Character05.png"),
        game->getTexture("assets/Character06.png") };

    asc->setAnimTextures(anims);
    asc->setAnimFPS(6);
}

void Character::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);
}

void Character::processKeyboard(const bool* state) {}

bool Character::getJumping() const {
    return jumping;
}

bool Character::getPunching() const {
    return punching;
}
