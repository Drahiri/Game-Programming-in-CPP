#ifndef FONT_H
#define FONT_H

#include "math.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <unordered_map>

class Texture;
class Game;

class Font {
public:
    Font(Game* game);
    ~Font();

    // Load/unload from a file
    bool load(const std::string& fileName);
    void unload();

    // Given string and this font, draw to a texture
    Texture* renderText(
          const std::string& text, const Vector3& color = Color::White, int pointSize = 30);

private:
    Game* game;
    // Map of point sized to font data
    std::unordered_map<int, TTF_Font*> fontData;
};

#endif