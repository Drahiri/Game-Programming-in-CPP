#include "level_loader.h"

#include "actor.h"
#include "audio_component.h"
#include "ball_actor.h"
#include "ball_move.h"
#include "box_component.h"
#include "camera_component.h"
#include "component.h"
#include "follow_actor.h"
#include "follow_camera.h"
#include "game.h"
#include "mesh_component.h"
#include "mirror_camera.h"
#include "move_component.h"
#include "plane_actor.h"
#include "point_light_component.h"
#include "renderer.h"
#include "skeletal_mesh_component.h"
#include "sprite_component.h"
#include "target_actor.h"
#include "target_component.h"

#include <fstream>
#include <SDL3/SDL_log.h>
#include <vector>

const int levelVersion = 1;

std::unordered_map<std::string, ActorFunc> LevelLoader::actorFactoryMap{
    // clang-format off
    { "Actor", &Actor::create<Actor> },
    { "BallActor", &Actor::create<BallActor> },
    { "FollowActor", &Actor::create<FollowActor> },
    { "PlaneActor", &Actor::create<PlaneActor> }, 
    { "TargetActor", &Actor::create<TargetActor> }
    // clang-format on
};

std::unordered_map<std::string, std::pair<Component::TypeID, ComponentFunc>>
      LevelLoader::componentFactoryMap{
          // clang-format off
    { "AudioComponent", { Component::TypeID::AudioComponent, &Component::create<AudioComponent> } },
    { "BallMove", { Component::TypeID::BallMove, &Component::create<BallMove> } },
    { "BoxComponent", { Component::TypeID::BoxComponent, &Component::create<BoxComponent> } },
    { "CameraComponent", { Component::TypeID::CameraComponent, &Component::create<CameraComponent> } },
    { "FollowCamera", { Component::TypeID::FollowCamera, &Component::create<FollowCamera> } },
    { "MeshComponent>", { Component::TypeID::MeshComponent, &Component::create<MeshComponent> } },
    { "MoveComponent>", { Component::TypeID::MoveComponent, &Component::create<MoveComponent> } },
    { "SkeletalMeshComponent>", { Component::TypeID::SkeletalMeshComponent, &Component::create<SkeletalMeshComponent> } },
    { "SpriteComponent>", { Component::TypeID::SpriteComponent, &Component::create<SpriteComponent> } },
    { "MirrorCamera>", { Component::TypeID::MirrorCamera, &Component::create<MirrorCamera> } },
    { "PointLightComponent>", { Component::TypeID::PointLightComponent, &Component::create<PointLightComponent> } },
    { "TargetComponent>", { Component::TypeID::TargetComponent, &Component::create<TargetComponent> } }
          // clang-format on
};

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

    const rapidjson::Value& globals = doc["globalProperties"];
    if(globals.IsObject()) {
        loadGloabalProperties(game, globals);
    }

    const rapidjson::Value& actors = doc["actors"];
    if(actors.IsArray()) {
        loadActors(game, actors);
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

void LevelLoader::loadGloabalProperties(Game* game, const rapidjson::Value& inObject) {
    // Get ambient light
    Vector3 ambient;
    if(JsonHelper::getVector3(inObject, "ambientLight", ambient)) {
        game->getRenderer()->setAmbientLight(ambient);
    }

    // Get directional light
    const rapidjson::Value& dirObj = inObject["directionalLight"];
    if(dirObj.IsObject()) {
        DirectionalLight& light = game->getRenderer()->getDirectionalLight();
        // Set direction/color, if they exist
        JsonHelper::getVector3(dirObj, "direction", light.direction);
        JsonHelper::getVector3(dirObj, "color", light.diffuseColor);
    }
}

void LevelLoader::loadActors(Game* game, const rapidjson::Value& inArray) {
    // Loop throughg array of actors
    for(rapidjson::SizeType i = 0; i < inArray.Size(); i++) {
        const rapidjson::Value& actorObj = inArray[i];
        if(actorObj.IsObject()) {
            // Get the type
            std::string type;

            if(JsonHelper::getString(actorObj, "type", type)) {
                // Is this type in the map?
                auto iter = actorFactoryMap.find(type);
                if(iter != actorFactoryMap.end()) {
                    // Construct with function stored in map
                    Actor* actor = iter->second(game, actorObj["properties"]);
                } else {
                    SDL_Log("Unknown actor type %s", type.c_str());
                }
            }
        }
    }
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

bool JsonHelper::getFloat(
      const rapidjson::Value& inObject, const char* inProperty, float& outFloat) {
    auto itr = inObject.FindMember(inProperty);
    if(itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if(!property.IsDouble()) {
        return false;
    }

    outFloat = property.GetDouble();

    return true;
}

bool JsonHelper::getBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool) {
    auto itr = inObject.FindMember(inProperty);
    if(itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if(!property.IsBool()) {
        return false;
    }

    outBool = property.GetBool();

    return true;
}

bool JsonHelper::getString(
      const rapidjson::Value& inObject, const char* inProperty, std::string& outString) {
    auto itr = inObject.FindMember(inProperty);
    if(itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if(!property.IsString()) {
        return false;
    }

    outString = property.GetString();

    return true;
}

bool JsonHelper::getVector3(
      const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector) {
    auto itr = inObject.FindMember(inProperty);
    if(itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if(!property.IsArray() || property.Size() != 3) {
        return false;
    }

    for(rapidjson::SizeType i = 0; i < 3; i++) {
        if(!property[i].IsDouble()) {
            return false;
        }
    }

    outVector.x = property[0].GetDouble();
    outVector.y = property[1].GetDouble();
    outVector.z = property[2].GetDouble();

    return true;
}

bool JsonHelper::getQuaternion(
      const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuaterion) {
    auto itr = inObject.FindMember(inProperty);
    if(itr == inObject.MemberEnd()) {
        return false;
    }

    auto& property = itr->value;
    if(!property.IsArray() || property.Size() != 4) {
        return false;
    }

    for(rapidjson::SizeType i = 0; i < 4; i++) {
        if(!property[i].IsDouble()) {
            return false;
        }
    }

    outQuaterion.x = property[0].GetDouble();
    outQuaterion.y = property[1].GetDouble();
    outQuaterion.z = property[2].GetDouble();
    outQuaterion.w = property[3].GetDouble();

    return true;
}
