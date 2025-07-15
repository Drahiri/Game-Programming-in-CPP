#include "font.h"

#include "texture.h"

#include <vector>

Font::Font(Game* game) : game(game) {}

Font::~Font() {}

bool Font::load(const std::string& fileName) {
    // Support these font sizes
    // clang-format off
    std::vector<int> fontSizes = { 8,
        9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28,
        30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 52, 56,
        58, 60, 64, 68, 72
    };
    // clang-format on
    // Call TTF_OpenFont once per every font size
    for(auto& size: fontSizes) {
        TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
        if(font == nullptr) {
            SDL_Log("Failed to load font %s in size %d", fileName.c_str(), size);
            return false;
        }

        fontData.emplace(size, font);
    }

    return true;
}

void Font::unload() {
    for(auto& font: fontData) {
        TTF_CloseFont(font.second);
    }
}

Texture* Font::renderText(const std::string& text, const Vector3& color, int pointSize) {
    return nullptr;
}
