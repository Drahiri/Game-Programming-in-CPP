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

class JsonHelper {
    // Helpers - Return true if successful, and also sets out parameter to parsed value
    // For each function, the first parameter is the containing JSON object, the second is the
    // name of the property in the containing object, and the third is the value you acquire.
    // Furthermore, if the property is not found, each function is guaranteed not to modify the
    // return value.

public:
    static bool getInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
};

#endif