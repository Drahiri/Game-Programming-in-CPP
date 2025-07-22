#include "main_menu.h"

#include "dialog_box.h"
#include "game.h"
#include "renderer.h"

MainMenu::MainMenu(Game* game) : UIScreen(game) {
    game->setState(Game::GameState::MainMenu);
    setTitle("MainTitle");
    addButton("PlayButton", [this]() { close(); });
    addButton("QuitButton", [this]() {
        new DialogBox(
              this->game, "QuitText", [this]() { this->game->setState(Game::GameState::Quit); });
    });

    game->getRenderer()->setRelativeMouse(false);
}

MainMenu::~MainMenu() {
    game->setState(Game::GameState::Gameplay);
    game->getRenderer()->setRelativeMouse(true);
}
