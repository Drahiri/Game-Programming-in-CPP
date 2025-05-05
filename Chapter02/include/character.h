#ifndef CHARACTER_H
#define CHARACTER_H

#include "actor.h"

class Game;

class Character : public Actor {
public:
    Character(Game* game);

    void updateActor(float deltaTime) override;
    void processKeyboard(const bool* state);

    bool getJumping() const;
    bool getPunching() const;

private:
    bool jumping;
    bool punching;
};

#endif