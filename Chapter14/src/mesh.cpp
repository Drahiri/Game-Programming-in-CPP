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

namespace {
union Vertex {
    float f;
    uint8_t b[4];
};

const uint32_t binaryVersion = 1;

struct MeshBinHeader {
    // Signature for file type
    char signature[4] = { 'G', 'M', 'S', 'H' };
    // Version
    uint32_t version = binaryVersion;
    // Vertex layout type
    VertexArray::Layout layout = VertexArray::Layout::PosNormTex;
    // Info about how many of each you have;
    uint32_t numTextures = 0;
    uint32_t numVerts = 0;
    uint32_t numIndices = 0;
    // Box/radius of mesh, used for collision
    AABB box{ Vector3::Zero, Vector3::Zero };
    float radius = 0.0f;
};
} // namespace

Mesh::Mesh() :
    vertexArray(nullptr),
    radius(0.0f),
    specPower(100.0f),
    box(Vector3::Infinity, Vector3::NegInfinity) {}

Mesh::~Mesh() {}

bool Mesh::load(const std::string& fileName, Renderer* renderer) {
    this->fileName = fileName;
    // Try loading the binary file first
    if(loadBinary(fileName + ".bin", renderer)) {
        return true;
    }

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
    std::string vertexFormat = doc["vertexformat"].GetString();

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

    std::vector<std::string> textureNames;

    for(rapidjson::SizeType i = 0; i < texJson.Size(); i++) {
        // Is this texture already loaded?
        std::string texName = texJson[i].GetString();
        textureNames.emplace_back(texName);
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

    std::vector<Vertex> vertices;
    vertices.reserve(vertsJson.Size() * vertSize);
    radius = 0.0f;
    for(rapidjson::SizeType i = 0; i < vertsJson.Size(); i++) {
        const rapidjson::Value& vert = vertsJson[i];
        if(!vert.IsArray()) {
            SDL_Log("Unexpected vertex format for %s", fileName.c_str());
            return false;
        }

        Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
        radius = Math::Max(radius, pos.LengthSq());
        box.updateMinMax(pos);

        if(vertexLayout == VertexArray::Layout::PosNormTex) {
            Vertex v;
            // Add the floats
            for(rapidjson::SizeType j = 0; j < vert.Size(); j++) {
                v.f = static_cast<float>(vert[j].GetDouble());
                vertices.emplace_back(v);
            }
        } else {
            Vertex v;
            // Add pos/normal
            for(rapidjson::SizeType j = 0; j < 6; j++) {
                v.f = static_cast<float>(vert[j].GetDouble());
                vertices.emplace_back(v);
            }

            // Add skin information
            for(rapidjson::SizeType j = 6; j < 14; j += 4) {
                v.b[0] = vert[j].GetUint();
                v.b[1] = vert[j + 1].GetUint();
                v.b[2] = vert[j + 2].GetUint();
                v.b[3] = vert[j + 3].GetUint();
                vertices.emplace_back(v);
            }

            // Add tex oords
            for(rapidjson::SizeType j = 14; j < vert.Size(); j++) {
                v.f = vert[j].GetDouble();
                vertices.emplace_back(v);
            }
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
    unsigned int numVerts = static_cast<unsigned>(vertices.size()) / vertSize;
    vertexArray = new VertexArray(vertexLayout,
          vertices.data(),
          numVerts,
          indices.data(),
          static_cast<unsigned>(indices.size()));

    // Reading spec power
    const rapidjson::Value& specJSON = doc["specularPower"];
    specPower = static_cast<float>(specJSON.GetDouble());

    // Save the binary mesh
    saveBinary(fileName + ".bin",
          vertices.data(),
          numVerts,
          vertexLayout,
          indices.data(),
          static_cast<unsigned>(indices.size()),
          textureNames,
          box,
          radius);

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

const std::string& Mesh::getFileName() const {
    return fileName;
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

bool Mesh::loadBinary(const std::string& fileName, Renderer* renderer) {
    std::ifstream inFile(fileName, std::ios::in | std::ios::binary);
    if(inFile.is_open()) {
        MeshBinHeader header;
        inFile.read(reinterpret_cast<char*>(&header), sizeof(header));

        // Validate the header signature and version
        char* sig = header.signature;
        if(sig[0] != 'G' || sig[1] != 'M' || sig[2] != 'S' || sig[3] != 'H'
              || header.version != binaryVersion) {
            return false;
        }

        // Read in the texture file names
        for(uint32_t i = 0; i < header.numTextures; i++) {
            // Get the file name size
            uint16_t nameSize = 0;
            inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));

            // Make a buffer of this size
            char* texName = new char[nameSize];
            // Read in the texture name
            inFile.read(texName, nameSize);

            // Get this texture
            Texture* t = renderer->getTexture(texName);
            if(t == nullptr) {
                // If it's null, use the default texture
                t = renderer->getTexture("assets/Default.png");
            }
            textures.emplace_back(t);
            delete[] texName;
        }

        // Now read in the vertices
        unsigned vertexSize = VertexArray::getVertexSize(header.layout);
        char* verts = new char[header.numVerts * vertexSize];
        inFile.read(verts, header.numVerts * vertexSize);

        // Now read in the indices
        uint32_t* indices = new uint32_t[header.numIndices];
        inFile.read(reinterpret_cast<char*>(indices), header.numIndices * sizeof(uint32_t));

        VertexArray::Layout layout = reinterpret_cast<VertexArray::Layout>(header.layout);

        // Now create the vertex array
        vertexArray = new VertexArray(layout, verts, header.numVerts, indices, header.numIndices);

        // Cleanup memory
        delete[] verts;
        delete[] indices;

        // Set box/radius/specular from header
        box = header.box;
        radius = header.radius;

        return true;
    }

    return false;
}

void Mesh::saveBinary(const std::string& fileName,
      const void* verts,
      uint32_t numVerts,
      VertexArray::Layout layout,
      const uint32_t* indices,
      uint32_t numIndices,
      const std::vector<std::string>& textureNames,
      const AABB& box,
      float radius) {
    // Create header struct
    MeshBinHeader header;
    header.layout = layout;
    header.numTextures = static_cast<unsigned>(textureNames.size());
    header.numVerts = numVerts;
    header.numIndices = numIndices;
    header.box = box;
    header.radius = radius;

    // Open binary file for writing
    std::ofstream outFile(fileName, std::ios::out | std::ios::binary);
    if(outFile.is_open()) {
        // Write the header
        outFile.write(reinterpret_cast<char*>(&header), sizeof(header));
        // For each texture, we need to write the size of the name
        // followed by the string, followed by null terminator
        for(const auto& tex: textureNames) {
            uint16_t nameSize = static_cast<uint16_t>(tex.length()) + 1;
            outFile.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
            outFile.write(tex.c_str(), nameSize - 1);
            outFile.write("\0", 1);
        }

        // Figure out number of bytes for each vertex, based on layout
        unsigned vertexSize = VertexArray::getVertexSize(layout);
        // Write vertices
        outFile.write(reinterpret_cast<const char*>(verts), numVerts * vertexSize);
        // Write indices
        outFile.write(reinterpret_cast<const char*>(indices), numIndices * sizeof(uint32_t));
    }
}