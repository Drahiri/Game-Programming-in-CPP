#include "pause_menu.h"

#include "dialog_box.h"
#include "game.h"
#include "renderer.h"

#include <SDL3/SDL.h>

PauseMenu::PauseMenu(Game* game) : UIScreen(game) {
    game->setState(Game::GameState::Paused);
    setTitle("PauseTitle");
    addButton("ResumeButton", [this]() { close(); });
    addButton("QuitButton", [this]() {
        new DialogBox(
              this->game, "QuitText", [this]() { this->game->setState(Game::GameState::Quit); });
    });

    game->getRenderer()->setRelativeMouse(false);
}

PauseMenu::~PauseMenu() {
    game->setState(Game::GameState::Gameplay);
    game->getRenderer()->setRelativeMouse(true);
}

void PauseMenu::handleKeyPress(int key) {
    UIScreen::handleKeyPress(key);
    if(key == SDLK_ESCAPE) {
        close();
    }
}
