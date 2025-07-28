#include "skeleton.h"

#include <fstream>
#include <rapidjson/document.h>
#include <SDL3/SDL_log.h>
#include <sstream>

bool Skeleton::load(const std::string& fileName) {
    std::ifstream file(fileName);
    if(!file.is_open()) {
        SDL_Log("File not found: Skeleton %s", fileName.c_str());
        return false;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);

    if(!doc.IsObject()) {
        SDL_Log("Skeleton %s is not valid json", fileName.c_str());
        return false;
    }

    int ver = doc["version"].GetInt();

    // Check the metadata
    if(ver != 1) {
        SDL_Log("Skeleton %s unknown format", fileName.c_str());
        return false;
    }

    const rapidjson::Value& bonecount = doc["bonecount"];
    if(!bonecount.IsUint()) {
        SDL_Log("Skeleton %s doesn't have a bone count", fileName.c_str());
        return false;
    }

    size_t count = bonecount.GetUint();
    // TODO: Change to MAX_SKELETON_BONES
    if(count > 96) {
        SDL_Log("Skeleton %s exceeds maximum bone count", fileName.c_str());
        return false;
    }

    bones.reserve(count);

    const rapidjson::Value& jsonBones = doc["bones"];
    if(!jsonBones.IsArray()) {
        SDL_Log("Skeleton %s doesn't have a bone array?", fileName.c_str());
        return false;
    }
    if(jsonBones.Size() != count) {
        SDL_Log("Skeleton %s has a mismatch between the bone count and number of bones",
              fileName.c_str());
        return false;
    }

    Bone temp;
    for(rapidjson::SizeType i = 0; i < count; i++) {
        if(!jsonBones[i].IsObject()) {
            SDL_Log("Skeleton %s: Bone %d is invalid", fileName.c_str(), i);
            return false;
        }

        const rapidjson::Value& name = jsonBones[i]["name"];
        temp.name = name.GetString();

        const rapidjson::Value& parent = jsonBones[i]["parent"];
        temp.parent = parent.GetInt();

        const rapidjson::Value& bindpose = jsonBones[i]["bindpose"];
        if(!bindpose.IsObject()) {
            SDL_Log("Skeleton %s: Bone %d is invalid", fileName.c_str(), i);
            return false;
        }

        const rapidjson::Value& rot = bindpose["rot"];
        const rapidjson::Value& trans = bindpose["trans"];

        if(!rot.IsArray() || !trans.IsArray()) {
            SDL_Log("Skeleton %s: Bone %d is invalid", fileName.c_str(), i);
            return false;
        }

        temp.localBindPose.rotation.x = rot[0].GetDouble();
        temp.localBindPose.rotation.y = rot[1].GetDouble();
        temp.localBindPose.rotation.z = rot[2].GetDouble();
        temp.localBindPose.rotation.w = rot[3].GetDouble();

        temp.localBindPose.translation.x = trans[0].GetDouble();
        temp.localBindPose.translation.y = trans[1].GetDouble();
        temp.localBindPose.translation.z = trans[2].GetDouble();

        bones.emplace_back(temp);
    }

    computeGlobalInvBindPose();

    return true;
}

void Skeleton::computeGlobalInvBindPose() {
    // Resize to number of bones, which automatically fills identity
    globalInvBindPoses.resize(getNumBones());

    // Step 1: Compute global bind pose for each bone
    // The global bind pose for root is just the local bind pose
    globalInvBindPoses[0] = bones[0].localBindPose.toMatrix();

    // Each remaining bone's global bind pose is it's local pose
    // multiplied by the parent's global bind pose
    for(size_t i = 1; i < globalInvBindPoses.size(); i++) {
        Matrix4 localMat = bones[i].localBindPose.toMatrix();
        globalInvBindPoses[i] = localMat * globalInvBindPoses[bones[i].parent];
    }

    // Step 2: invert each matrix
    for(size_t i = 0; i < globalInvBindPoses.size(); i++) {
        globalInvBindPoses[i].Invert();
    }
}

size_t Skeleton::getNumBones() const {
    return bones.size();
}

const Skeleton::Bone& Skeleton::getBone(size_t idx) const {
    return bones[idx];
}

const std::vector<Skeleton::Bone>& Skeleton::getBones() const {
    return bones;
}

const std::vector<Matrix4>& Skeleton::getGlobalInvBindPoses() const {
    return globalInvBindPoses;
}
