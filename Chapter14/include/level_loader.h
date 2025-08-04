#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include <rapidjson/document.h>
#include <string>

class Game;

class LevelLoader {
public:
    // Load the level - returns true if successful
    static bool loadLevel(Game* game, const std::string& fileName);

    static bool loadJSON(const std::string& fileName, rapidjson::Document& outDoc);
};

#endif