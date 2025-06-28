#include "asteroid.h"

#include "circle_component.h"
#include "game.h"
#include "move_component.h"
#include "random.h"
#include "sprite_component.h"

Asteroid::Asteroid(Game* game) : Actor(game) {
    // Initialize to random position/orientation
    Vector2 screenSize = game->getScreenSize();
    Vector2 halfScreen = Vector2(screenSize.x / 2, screenSize.y / 2);
    Vector2 negativeHalfScreen = Vector2(-screenSize.x / 2, -screenSize.y / 2);

    Vector2 randPos = Random::GetVector(negativeHalfScreen, halfScreen);
    setPosition(randPos);
    setRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

    // Create a sprite component and set texture
    SpriteComponent* sc = new SpriteComponent(this);
    sc->setTexture(game->getTexture("assets/Asteroid.png"));

    // Create a move component, andd set a forward speed
    MoveComponent* mc = new MoveComponent(this);
    mc->setForwardSpeed(150.0f);

    // Create circle component
    circle = new CircleComponent(this);
    circle->setRadius(40.0f);

    game->addAsteroid(this);
}

Asteroid::~Asteroid() {
    getGame()->removeAsteroid(this);
}

CircleComponent* Asteroid::getCircle() const {
    return circle;
}
