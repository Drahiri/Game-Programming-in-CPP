#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include "ui_screen.h"

#include <functional>
#include <string>

class Game;

class DialogBox : public UIScreen {
public:
    DialogBox(Game* game, const std::string& text, std::function<void()> onOK);
};

#endif