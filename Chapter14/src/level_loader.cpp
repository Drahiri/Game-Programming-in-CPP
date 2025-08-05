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
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
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
    { "MeshComponent", { Component::TypeID::MeshComponent, &Component::create<MeshComponent> } },
    { "MoveComponent", { Component::TypeID::MoveComponent, &Component::create<MoveComponent> } },
    { "SkeletalMeshComponent", { Component::TypeID::SkeletalMeshComponent, &Component::create<SkeletalMeshComponent> } },
    { "SpriteComponent", { Component::TypeID::SpriteComponent, &Component::create<SpriteComponent> } },
    { "MirrorCamera", { Component::TypeID::MirrorCamera, &Component::create<MirrorCamera> } },
    { "PointLightComponent", { Component::TypeID::PointLightComponent, &Component::create<PointLightComponent> } },
    { "TargetComponent", { Component::TypeID::TargetComponent, &Component::create<TargetComponent> } }
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

void LevelLoader::saveLevel(Game* game, const std::string& fileName) {
    // Create the document and root object
    rapidjson::Document doc;
    doc.SetObject();

    // Write the version
    JsonHelper::addInt(doc.GetAllocator(), doc, "version", 1);

    // TODO: Create the rest of the file
    // Saving global light properties
    rapidjson::Value globals(rapidjson::kObjectType);
    saveGlobalProperties(doc.GetAllocator(), game, globals);
    doc.AddMember("globalProperties", globals, doc.GetAllocator());

    // Save JSON to string buffer
    rapidjson::StringBuffer buffer;
    // Use PrettyWriter for pretty output (otherwise use Writer)
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char* output = buffer.GetString();

    // Write output to file
    std::ofstream outFile(fileName);
    if(outFile.is_open()) {
        outFile << output;
    }
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
        if(!actorObj.IsObject()) {
            continue;
        }
        // Get the type
        std::string type;
        if(!JsonHelper::getString(actorObj, "type", type)) {
            continue;
        }
        // Is this type in the map?
        auto iter = actorFactoryMap.find(type);
        if(iter == actorFactoryMap.end()) {
            SDL_Log("Unknown actor type %s", type.c_str());
            continue;
        }
        // Construct with function stored in map
        Actor* actor = iter->second(game, actorObj["properties"]);
        // Get the actor's components
        if(actorObj.HasMember("components")) {
            const rapidjson::Value& components = actorObj["components"];
            if(components.IsArray()) {
                loadComponents(actor, components);
            }
        }
    }
}

void LevelLoader::loadComponents(Actor* actor, const rapidjson::Value& inArray) {
    // Loop through array of components
    for(rapidjson::SizeType i = 0; i < inArray.Size(); i++) {
        const rapidjson::Value& compObj = inArray[i];
        if(!compObj.IsObject()) {
            continue;
        }

        // Get the type
        std::string type;
        if(!JsonHelper::getString(compObj, "type", type)) {
            continue;
        }

        auto iter = componentFactoryMap.find(type);
        if(iter == componentFactoryMap.end()) {
            SDL_Log("Unknown component type %s", type.c_str());
            continue;
        }
        // Get the typeid of component
        Component::TypeID tid = iter->second.first;
        // Does the actor already have a component of this type?
        Component* comp = actor->getComponentOfType(tid);
        if(comp == nullptr) {
            // It's a new component, call function from map
            comp = iter->second.second(actor, compObj["properties"]);
        } else {
            // It's already exists, just load properties
            comp->loadProperties(compObj["properties"]);
        }
    }
}

void LevelLoader::saveGlobalProperties(
      rapidjson::Document::AllocatorType& alloc, Game* game, rapidjson::Value& inObject) {
    // Ambient light
    JsonHelper::addVector3(alloc, inObject, "ambientLight", game->getRenderer()->getAmbientLight());

    // Directional light
    DirectionalLight& dirLight = game->getRenderer()->getDirectionalLight();
    rapidjson::Value dirObj(rapidjson::kObjectType);
    JsonHelper::addVector3(alloc, dirObj, "direction", dirLight.direction);
    JsonHelper::addVector3(alloc, dirObj, "color", dirLight.diffuseColor);
    inObject.AddMember("directionalLight", inObject, alloc);
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

void JsonHelper::addInt(rapidjson::Document::AllocatorType& alloc,
      rapidjson::Value& inObject,
      const char* name,
      int value) {
    rapidjson::Value v(value);
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::addFloat(rapidjson::Document::AllocatorType& alloc,
      rapidjson::Value& inObject,
      const char* name,
      float value) {
    rapidjson::Value v(value);
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::addBool(rapidjson::Document::AllocatorType& alloc,
      rapidjson::Value& inObject,
      const char* name,
      bool value) {
    rapidjson::Value v(value);
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::addString(rapidjson::Document::AllocatorType& alloc,
      rapidjson::Value& inObject,
      const char* name,
      const std::string& value) {
    rapidjson::Value v;
    v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::addVector3(rapidjson::Document::AllocatorType& alloc,
      rapidjson::Value& inObject,
      const char* name,
      const Vector3& value) {
    // Create an array
    rapidjson::Value v(rapidjson::kArrayType);
    // Push back elements
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);
    v.PushBack(rapidjson::Value(value.z).Move(), alloc);

    // Add array to inObject
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::addQuaternion(rapidjson::Document::AllocatorType& alloc,
      rapidjson::Value& inObject,
      const char* name,
      const Quaternion& value) {
    // Create an array
    rapidjson::Value v(rapidjson::kArrayType);
    // Push back elements
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);
    v.PushBack(rapidjson::Value(value.z).Move(), alloc);
    v.PushBack(rapidjson::Value(value.w).Move(), alloc);

    // Add array to inObject
    inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}
