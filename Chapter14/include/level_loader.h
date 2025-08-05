#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "component.h"
#include "math.h"

#include <functional>
#include <rapidjson/document.h>
#include <string>
#include <unordered_map>

class Actor;
class Game;

using ActorFunc = std::function<Actor*(Game*, const rapidjson::Value&)>;
using ComponentFunc = std::function<Component*(Actor*, const rapidjson::Value&)>;

class LevelLoader {
public:
    // Load the level - returns true if successful
    static bool loadLevel(Game* game, const std::string& fileName);
    static bool loadJSON(const std::string& fileName, rapidjson::Document& outDoc);

    // Saving the level
    static void saveLevel(Game* game, const std::string& fileName);

protected:
    // Helper to load global properties
    static void loadGloabalProperties(Game* game, const rapidjson::Value& inObject);
    static void loadActors(Game* game, const rapidjson::Value& inObject);
    static void loadComponents(Actor* actor, const rapidjson::Value& inArray);
    // Maps for data
    static std::unordered_map<std::string, ActorFunc> actorFactoryMap;
    static std::unordered_map<std::string, std::pair<Component::TypeID, ComponentFunc>>
          componentFactoryMap;
};

class JsonHelper {
    // Helpers - Return true if successful, and also sets out parameter to parsed value
    // For each function, the first parameter is the containing JSON object, the second is the
    // name of the property in the containing object, and the third is the value you acquire.
    // Furthermore, if the property is not found, each function is guaranteed not to modify the
    // return value.

public:
    static bool getInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
    static bool getFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
    static bool getBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
    static bool getString(
          const rapidjson::Value& inObject, const char* inProperty, std::string& outString);
    static bool getVector3(
          const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector);
    static bool getQuaternion(
          const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuaterion);

    // Setters
    static void addInt(rapidjson::Document::AllocatorType& alloc,
          rapidjson::Value& inObject,
          const char* name,
          int value);
    static void setFloat(rapidjson::Document::AllocatorType& alloc,
          rapidjson::Value& inObject,
          const char* name,
          float value);
    static void setBool(rapidjson::Document::AllocatorType& alloc,
          rapidjson::Value& inObject,
          const char* name,
          bool value);
    static void setString(rapidjson::Document::AllocatorType& alloc,
          rapidjson::Value& inObject,
          const char* name,
          const std::string& value);
    static void setVector3(rapidjson::Document::AllocatorType& alloc,
          rapidjson::Value& inObject,
          const char* name,
          const Vector3& value);
    static void setQuaternion(rapidjson::Document::AllocatorType& alloc,
          rapidjson::Value& inObject,
          const char* name,
          const Quaternion& value);
};

#endif