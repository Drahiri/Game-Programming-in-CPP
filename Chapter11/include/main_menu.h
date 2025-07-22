#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "ui_screen.h"

class Game;

class MainMenu : public UIScreen {
public:
    MainMenu(Game* game);
    ~MainMenu();
};

#endif