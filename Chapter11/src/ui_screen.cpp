#include "ui_screen.h"

UIScreen::UIScreen(Game* game) :
    game(game),
    font(nullptr),
    title(nullptr),
    titlePos(Vector2::Zero),
    state(UIState::Active) {}

void UIScreen::close() {
    state = UIState::Closing;
}

UIScreen::UIState UIScreen::getState() const {
    return state;
}
