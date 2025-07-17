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

UIScreen::~UIScreen() {}

void UIScreen::update(float deltaTime) {}

void UIScreen::draw(Shader* shader) {}

void UIScreen::processInput(const bool* keys) {}

void UIScreen::handleKeyPress(int key) {}

void UIScreen::close() {
    state = UIState::Closing;
}

UIScreen::UIState UIScreen::getState() const {
    return state;
}

void UIScreen::setTitle(const std::string& text, const Vector3& color, int pointSize) {}

void UIScreen::drawTexture(Shader* shader, Texture* texture, const Vector2& offset, float scale) {}
