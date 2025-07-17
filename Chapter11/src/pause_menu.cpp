#include "pause_menu.h"

#include "game.h"

PauseMenu::PauseMenu(Game* game) : UIScreen(game) {
    game->setState(Game::GameState::Paused);
    setTitle("PAUSED");
}

PauseMenu::~PauseMenu() {
    game->setState(Game::GameState::Gameplay);
}

void PauseMenu::handleKeyPress(int key) {
    UIScreen::handleKeyPress(key);
    if(key == SDLK_ESCAPE) {
        close();
    }
}
