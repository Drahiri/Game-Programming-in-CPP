#include "mesh.h"

#include "game.h"
#include "math.h"
#include "renderer.h"
#include "texture.h"
#include "vertex_array.h"

#include <fstream>
#include <rapidjson/document.h>
#include <SDL3/SDL_log.h>
#include <sstream>

Mesh::Mesh() :
    vertexArray(nullptr),
    radius(0.0f),
    specPower(100.0f),
    box(Vector3::Infinity, Vector3::NegInfinity) {}

Mesh::~Mesh() {}

bool Mesh::load(const std::string& fileName, Renderer* renderer) {
    std::ifstream file(fileName);
    if(!file.is_open()) {
        SDL_Log("File not found: Mesh %s", fileName.c_str());
        return false;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);

    if(!doc.IsObject()) {
        SDL_Log("Mesh %s is not valid json", fileName.c_str());
        return false;
    }

    int ver = doc["version"].GetInt();
    if(ver != 1) {
        SDL_Log("Mesh %s not version 1", fileName.c_str());
        return false;
    }

    shaderName = doc["shader"].GetString();

    VertexArray::Layout vertexLayout = VertexArray::Layout::PosNormTex;
    size_t vertSize = 8;
    std::string vertexFormat = doc["vertexFormat"].GetString();

    if(vertexFormat == "PosNormSkinTex") {
        vertexLayout = VertexArray::Layout::PosNormSkinTex;
        vertSize = 10;
    }

    // Load textures
    const rapidjson::Value& texJson = doc["textures"];
    if(!texJson.IsArray() || texJson.Size() < 1) {
        SDL_Log("Mesh %s has no texJson, there should be at least one", fileName.c_str());
        return false;
    }

    for(rapidjson::SizeType i = 0; i < texJson.Size(); i++) {
        // Is this texture already loaded?
        std::string texName = texJson[i].GetString();
        Texture* t = renderer->getTexture(texName);
        if(t == nullptr) {
            // Try loading the texture
            t = renderer->getTexture(texName);
            if(t == nullptr) {
                // If it's still null, just use the default texture
                t = renderer->getTexture("assets/Default.png");
            }
        }
        textures.emplace_back(t);
    }

    // Load in the vertices
    const rapidjson::Value& vertsJson = doc["vertices"];
    if(!vertsJson.IsArray() || vertsJson.Size() < 1) {
        SDL_Log("Mesh %s has no vertices", fileName.c_str());
        return false;
    }

    std::vector<float> vertices;
    vertices.reserve(vertsJson.Size() * vertSize);
    radius = 0.0f;
    for(rapidjson::SizeType i = 0; i < vertsJson.Size(); i++) {
        // For now, just assume we have 8 elements
        const rapidjson::Value& vert = vertsJson[i];
        if(!vert.IsArray() || vert.Size() != 8) {
            SDL_Log("Unexpected vertex format for %s", fileName.c_str());
            return false;
        }

        Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
        radius = Math::Max(radius, pos.LengthSq());

        box.updateMinMax(pos);

        // Add the floats
        for(rapidjson::SizeType i = 0; i < vert.Size(); i++) {
            vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
        }
    }

    // We were computing length squared ealier
    radius = Math::Sqrt(radius);

    // Load in th indices
    const rapidjson::Value& indJson = doc["indices"];
    if(!indJson.IsArray() || indJson.Size() < 1) {
        SDL_Log("Mesh %s has no indices", fileName.c_str());
        return false;
    }
    std::vector<unsigned int> indices;
    indices.reserve(indJson.Size() * 3);
    for(rapidjson::SizeType i = 0; i < indJson.Size(); i++) {
        const rapidjson::Value& ind = indJson[i];
        if(!ind.IsArray() || ind.Size() < 1) {
            SDL_Log("Invalid indices for %s", fileName.c_str());
            return false;
        }

        indices.emplace_back(ind[0].GetUint());
        indices.emplace_back(ind[1].GetUint());
        indices.emplace_back(ind[2].GetUint());
    }

    // Now create a vertex array
    vertexArray = new VertexArray(vertexLayout,
          vertices.data(),
          static_cast<unsigned>(vertices.size()) / vertSize,
          indices.data(),
          static_cast<unsigned>(indices.size()));

    // Reading spec power
    const rapidjson::Value& specJSON = doc["specularPower"];
    specPower = static_cast<float>(specJSON.GetDouble());

    return true;
}

void Mesh::unload() {
    delete vertexArray;
    vertexArray = nullptr;
}

VertexArray* Mesh::getVertexArray() {
    return vertexArray;
}

Texture* Mesh::getTexture(size_t index) {
    if(index < textures.size()) {
        return textures[index];
    } else {
        return nullptr;
    }
}

const std::string& Mesh::getShaderName() const {
    return shaderName;
}

float Mesh::getRadius() const {
    return radius;
}

float Mesh::getSpecPower() const {
    return specPower;
}

const AABB& Mesh::getBox() const {
    return box;
}