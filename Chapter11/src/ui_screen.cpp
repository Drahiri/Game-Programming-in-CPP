#include "ui_screen.h"

#include "game.h"

UIScreen::UIScreen(Game* game) :
    game(game),
    font(nullptr),
    title(nullptr),
    titlePos(Vector2::Zero),
    state(UIState::Active) {
    game->pushUI(this);
}

void UIScreen::close() {
    state = UIState::Closing;
}

UIScreen::UIState UIScreen::getState() const {
    return state;
}
