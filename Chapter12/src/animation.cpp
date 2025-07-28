#include "animation.h"

#include "skeleton.h"

#include <fstream>
#include <rapidjson/document.h>
#include <SDL3/SDL_log.h>
#include <sstream>

bool Animation::load(const std::string& fileName) {
    std::ifstream file(fileName);
    if(!file.is_open()) {
        SDL_Log("Failed to found: Animation %s", fileName.c_str());
        return false;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);

    if(!doc.IsObject()) {
        SDL_Log("Animation %s is not valid json", fileName.c_str());
        return false;
    }

    int ver = doc["version"].GetInt();

    // Check the metadata
    if(ver != 1) {
        SDL_Log("Animation %s unkown format", fileName.c_str());
        return false;
    }

    const rapidjson::Value& sequence = doc["sequence"];
    if(!sequence.IsObject()) {
        SDL_Log("Animation %s doesn't have a sequence", fileName.c_str());
        return false;
    }

    const rapidjson::Value& frames = sequence["frames"];
    const rapidjson::Value& length = sequence["length"];
    const rapidjson::Value& bonecount = sequence["bonecount"];

    if(!frames.IsUint() || !length.IsDouble() || !bonecount.IsUint()) {
        SDL_Log("Sequence %s has invalid frames, length, or bone count", fileName.c_str());
        return false;
    }

    numFrames = frames.GetUint();
    duration = length.GetDouble();
    numBones = bonecount.GetUint();
    frameDuration = duration / (numFrames - 1);

    tracks.resize(numBones);

    const rapidjson::Value& jsonTracks = sequence["tracks"];

    if(!jsonTracks.IsArray()) {
        SDL_Log("Sequence %s missing a tracks array.", fileName.c_str());
        return false;
    }

    for(rapidjson::SizeType i = 0; i < jsonTracks.Size(); i++) {
        if(!jsonTracks[i].IsObject()) {
            SDL_Log("Animation %s: Track element %d is invalid", fileName.c_str(), i);
            return false;
        }

        size_t boneIndex = jsonTracks[i]["bone"].GetUint();

        const rapidjson::Value& transforms = jsonTracks[i]["transforms"];
        if(!transforms.IsArray()) {
            SDL_Log("Animation %s: Track element %d is missing transforms", fileName.c_str(), i);
            return false;
        }

        BoneTransform temp;
        if(transforms.Size() < numFrames) {
            SDL_Log("Animation %s: Track element %d has fewer frames than expected.",
                  fileName.c_str(),
                  i);
            return false;
        }

        for(rapidjson::SizeType j = 0; j < transforms.Size(); j++) {
            const rapidjson::Value& rot = transforms[j]["rot"];
            const rapidjson::Value& trans = transforms[j]["trans"];

            if(!rot.IsArray() || !trans.IsArray()) {
                SDL_Log("Skeleton %s: Bone %d is invalid", fileName.c_str(), i);
                return false;
            }

            temp.rotation.x = rot[0].GetDouble();
            temp.rotation.y = rot[1].GetDouble();
            temp.rotation.z = rot[2].GetDouble();
            temp.rotation.w = rot[3].GetDouble();

            temp.translation.x = trans[0].GetDouble();
            temp.translation.y = trans[1].GetDouble();
            temp.translation.z = trans[2].GetDouble();

            tracks[boneIndex].emplace_back(temp);
        }
    }

    return true;
}

size_t Animation::getNumBones() const {
    return numBones;
}

size_t Animation::getNumFrames() const {
    return numFrames;
}

float Animation::getDuration() const {
    return duration;
}

float Animation::getFrameDuration() const {
    return frameDuration;
}

void Animation::getGlobalPoseAtTime(
      std::vector<Matrix4>& outPoses, Skeleton* inSkeleton, float inTime) const {
    // Resize the outPoses vector if needed
    if(outPoses.size() != numBones) {
        outPoses.resize(numBones);
    }

    // For now, just compute the pose for every bone at frame 0
    const int frame = 0;
    // Set the pose for the root
    // Does the root have a track?
    if(tracks[0].size() > 0) {
        // The global pose for the root is just its local pose
        outPoses[0] = tracks[0][frame].toMatrix();
    } else {
        outPoses[0] = Matrix4::Identity;
    }

    const std::vector<Skeleton::Bone>& bones = inSkeleton->getBones();
    // Now compute the global pose matrices for every other bone
    for(size_t bone = 1; bone < numBones; bone++) {
        Matrix4 localMat; // Defaults to identity
        if(tracks[bone].size() > 0) {
            localMat = tracks[bone][frame].toMatrix();
        }
        outPoses[bone] = localMat * outPoses[bones[bone].parent];
    }
}