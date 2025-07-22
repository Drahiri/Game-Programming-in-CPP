#include "pause_menu.h"

#include "dialog_box.h"
#include "game.h"
#include "renderer.h"

#include <SDL3/SDL.h>

PauseMenu::PauseMenu(Game* game) : UIScreen(game) {
    game->setState(Game::GameState::Paused);
    setTitle("PauseTitle");
    addButton("ResumeButton", [this]() {
        this->game->setState(Game::GameState::Gameplay);
        this->game->getRenderer()->setRelativeMouse(true);
        close();
    });
    addButton("QuitButton", [this]() {
        new DialogBox(this->game, "QuitText", [this]() {
            close();
            this->game->createMainMenu();
        });
    });

    game->getRenderer()->setRelativeMouse(false);
}

PauseMenu::~PauseMenu() {}

void PauseMenu::handleKeyPress(int key) {
    UIScreen::handleKeyPress(key);
    if(key == SDLK_ESCAPE) {
        game->setState(Game::GameState::Gameplay);
        game->getRenderer()->setRelativeMouse(true);
        close();
    }
}
