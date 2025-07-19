#include "ui_screen.h"

#include "button.h"
#include "font.h"
#include "game.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"

UIScreen::UIScreen(Game* game) :
    game(game),
    title(nullptr),
    titlePos(0.0f, 300.0f),
    nextButtonPos(0.0f, 200.0f),
    background(nullptr),
    bgPos(Vector2::Zero),
    state(UIState::Active) {
    game->pushUI(this);
    font = game->getFont("assets/Carlito-Regular.ttf");
    buttonOn = game->getRenderer()->getTexture("assets/ButtonYellow.png");
    buttonOff = game->getRenderer()->getTexture("assets/ButtonBlue.png");
}

UIScreen::~UIScreen() {}

void UIScreen::update(float deltaTime) {}

void UIScreen::draw(Shader* shader) {
    // Draw title if exists
    if(title) {
        drawTexture(shader, title, titlePos);
    }

    for(auto b: buttons) {
        // Draw background of button
        Texture* tex = b->getHighlighted() ? buttonOn : buttonOff;
        drawTexture(shader, tex, b->getPosition());

        // Draw text of button
        drawTexture(shader, b->getNameTex(), b->getPosition());
    }
}

void UIScreen::processInput(const bool* keys) {
    // Are there buttons?
    if(!buttons.empty()) {
        // Get position of mouse
        float x, y;
        SDL_GetMouseState(&x, &y);
        // Convert to (0,0) center coordinates (assume 1024x768)
        Vector2 mousePos(x, y);
        mousePos.x -= game->getRenderer()->getScreenWidth() * 0.5f;
        mousePos.y = game->getRenderer()->getScreenHeight() * 0.5f - mousePos.y;

        // Highlight any buttons
        for(auto b: buttons) {
            if(b->containsPoint(mousePos)) {
                b->setHighlighted(true);
            } else {
                b->setHighlighted(false);
            }
        }
    }
}

void UIScreen::handleKeyPress(int key) {
    switch(key) {
    case SDL_BUTTON_LEFT:
        if(!buttons.empty()) {
            for(auto b: buttons) {
                if(b->getHighlighted()) {
                    b->onClick();
                    break;
                }
            }
        }
        break;
    default:
        break;
    }
}

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

void UIScreen::addButton(const std::string& name, std::function<void()> onClick) {
    Vector2 dims(
          static_cast<float>(buttonOn->getWidth()), static_cast<float>(buttonOn->getHeight()));
    Button* b = new Button(name, font, onClick, nextButtonPos, dims);
    buttons.emplace_back(b);
    // Update position of next button
    // Move down, by height on button plus padding
    nextButtonPos.y -= buttonOff->getHeight() + 20.0f;
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
