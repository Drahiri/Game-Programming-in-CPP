#include "asteroid.h"

#include "circle_component.h"
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
    mc = new MoveComponent(this);
    mc->setWrapable(true);
    mc->setMass(10.0f);

    // Create circle component
    circle = new CircleComponent(this);
    circle->setRadius(40.0f);

    game->addAsteroid(this);
}

Asteroid::~Asteroid() {
    getGame()->removeAsteroid(this);
}

void Asteroid::updateActor(float deltaTime) {
    mc->addForce(getForward() * 1500.0f);
}

CircleComponent* Asteroid::getCircle() const {
    return circle;
}
