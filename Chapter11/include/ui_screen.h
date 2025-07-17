#ifndef UI_SCREEN_H
#define UI_SCREEN_H

#include "math.h"

#include <string>

class Font;
class Game;
class Shader;
class Texture;

class UIScreen {
public:
    UIScreen(Game* game);
    virtual ~UIScreen();

    // UIScreen subclasses can override these
    virtual void update(float deltaTime);
    virtual void draw(Shader* shader);
    virtual void processInput(const bool* keys);
    virtual void handleKeyPress(int key);

    // Track if the UI is active or closing
    enum class UIState {
        Active,
        Closing
    };

    // Set state to closing
    void close();
    // Get state of UI screen
    UIState getState() const;
    // Change the title text
    void setTitle(const std::string& text, const Vector3& color = Color::White, int pointSize = 40);

protected:
    // Helper to draw a texture
    void drawTexture(Shader* shader,
          Texture* texture,
          const Vector2& offset = Vector2::Zero,
          float scale = 1.0f);

    Game* game;
    // For the UI screen's title text
    Font* font;
    Texture* title;
    Vector2 titlePos;

    // State
    UIState state;
};

#endif