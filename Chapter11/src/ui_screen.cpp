#include "ui_screen.h"

#include "font.h"
#include "game.h"
#include "shader.h"
#include "texture.h"

UIScreen::UIScreen(Game* game) :
    game(game),
    title(nullptr),
    titlePos(0.0f, 300.0f),
    state(UIState::Active) {
    game->pushUI(this);
    font = game->getFont("assets/Carlito-Regular.ttf");
}

UIScreen::~UIScreen() {}

void UIScreen::update(float deltaTime) {}

void UIScreen::draw(Shader* shader) {
    // Draw title if exists
    if(title) {
        drawTexture(shader, title, titlePos);
    }
}

void UIScreen::processInput(const bool* keys) {}

void UIScreen::handleKeyPress(int key) {}

void UIScreen::close() {
    state = UIState::Closing;
}

UIScreen::UIState UIScreen::getState() const {
    return state;
}

void UIScreen::setTitle(const std::string& text, const Vector3& color, int pointSize) {
    // Clear out previous title texture if exists
    if(title) {
        title->unload();
        delete title;
        title = nullptr;
    }
    title = font->renderText(text, color, pointSize);
}

void UIScreen::drawTexture(Shader* shader, Texture* texture, const Vector2& offset, float scale) {
    // Scale the quad by the width/height of texture
    Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(texture->getWidth()) * scale,
          static_cast<float>(texture->getHeight()) * scale,
          1.0f);
    // Translate to position on screen
    Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.x, offset.y, 0.0f));
    // Set world transform
    Matrix4 world = scaleMat * transMat;
    shader->setMatrixUniform("uWorldTransform", world);
    // Set current texture
    texture->setActive();
    // Draw quad
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
