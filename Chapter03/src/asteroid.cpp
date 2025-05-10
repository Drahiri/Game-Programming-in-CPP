#include "asteroid.h"

#include "game.h"
#include "move_component.h"
#include "random.h"
#include "sprite_component.h"

Asteroid::Asteroid(Game* game) : Actor(game) {
    // Initialize to random position/orientation
    Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(game->getScreenSize()));
    setPosition(randPos);
    setRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

    // Create a sprite component and set texture
    SpriteComponent* sc = new SpriteComponent(this);
    sc->setTexture(game->getTexture("assets/Asteroid.png"));

    // Create a move component, andd set a forward speed
    MoveComponent* mc = new MoveComponent(this);
    mc->setForwardSpeed(150.0f);
}