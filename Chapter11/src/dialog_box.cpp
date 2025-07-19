#include "dialog_box.h"

#include "button.h"
#include "game.h"
#include "renderer.h"

DialogBox::DialogBox(Game* game, const std::string& text, std::function<void()> onOK) :
    UIScreen(game) {
    // Adjust position for dialog box
    bgPos = Vector2(0.0f, 0.0f);
    titlePos = Vector2(0.0f, 100.0f);
    nextButtonPos = Vector2(0.0f, 0.0f);
    // Set background texture
    background = game->getRenderer()->getTexture("assets/DialogBG.png");
    setTitle(text, Vector3::Zero, 30);
    // Setup buttons
    addButton("OK", [onOK]() { onOK(); });
    addButton("Cancel", [this]() { close(); });
}