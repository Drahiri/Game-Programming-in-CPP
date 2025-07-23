#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "ui_screen.h"

class Game;

class PauseMenu : public UIScreen {
public:
    PauseMenu(Game* game);
    ~PauseMenu();

    void handleKeyPress(int key) override;
};

#endif