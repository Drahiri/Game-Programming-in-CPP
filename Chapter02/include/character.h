#ifndef CHARACTER_H
#define CHARACTER_H

#include "actor.h"

class Game;
class AnimSpriteComponent;

class Character : public Actor {
public:
    Character(Game* game);
    ~Character();

    void updateActor(float deltaTime) override;
    void processKeyboard(const bool* state);

    bool getJumping() const;
    bool getPunching() const;

private:
    AnimSpriteComponent* asc;

    bool jumping;
    bool punching;
};

#endif