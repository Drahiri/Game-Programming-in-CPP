#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include <rapidjson/document.h>
#include <string>

class Game;

class LevelLoader {
public:
    static bool loadJSON(const std::string& fileName, rapidjson::Document& outDoc);
};

#endif