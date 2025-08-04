#include "level_loader.h"

#include <fstream>
#include <SDL3/SDL_log.h>
#include <vector>

const int levelVersion = 1;

bool LevelLoader::loadLevel(Game* game, const std::string& fileName) {
    rapidjson::Document doc;
    if(!loadJSON(fileName, doc)) {
        SDL_Log("Failed to load level %s", fileName.c_str());
        return false;
    }

    int version = 0;
    if(!JsonHelper::getInt(doc, "version", version) || version != levelVersion) {
        SDL_Log("Incorrect level file version for %s", fileName.c_str());
        return false;
    }

    return true;
}

bool LevelLoader::loadJSON(const std::string& fileName, rapidjson::Document& outDoc) {
    // Load the file from disk into an ifstream in binary mode,
    // loaded with stream buffer at the end (ate)
    std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
    if(!file.is_open()) {
        SDL_Log("File %s not found", fileName.c_str());
        return false;
    }

    // Get the size of the file
    std::ifstream::pos_type fileSize = file.tellg();
    // Seek back to start of file
    file.seekg(0, std::ios::beg);

    // Create a vector of size + 1 (for null terminator)
    std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
    // Read in bytes into vector
    file.read(bytes.data(), static_cast<size_t>(fileSize));

    // Load raw data into RapidJSON document
    outDoc.Parse(bytes.data());
    if(!outDoc.IsObject()) {
        SDL_Log("File %s is not valid JSON", fileName.c_str());
        return false;
    }

    return true;
}

bool JsonHelper::getInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt) {
    // Check if this property exists
    auto itr = inObject.FindMember(inProperty);
    if(itr == inObject.MemberEnd()) {
        return false;
    }

    // Get the value type, and check it's an integer
    auto& property = itr->value;
    if(!property.IsInt()) {
        return false;
    }

    // We have the property
    outInt = property.GetInt();

    return true;
}
