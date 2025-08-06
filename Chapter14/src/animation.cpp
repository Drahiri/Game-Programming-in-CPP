#include "animation.h"

#include "level_loader.h"
#include "skeleton.h"

#include <fstream>
#include <rapidjson/document.h>
#include <SDL3/SDL_log.h>
#include <sstream>

namespace {
const uint32_t binaryVersion = 1;

struct AnimationBinHeader {
    char signature[4] = { 'G', 'M', 'S', 'H' };
    uint32_t version = binaryVersion;
    uint32_t frames = 0;
    float length = 0.0f;
    uint32_t boneCount = 0;
};

} // namespace

bool Animation::load(const std::string& fileName) {
    this->fileName = fileName;

    if(loadBinary(fileName + ".bin")) {
        return true;
    }

    rapidjson::Document doc;
    if(!LevelLoader::loadJSON(fileName, doc)) {
        SDL_Log("Failed to found: Animation %s", fileName.c_str());
        return false;
    }

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

    // Save the binary animation
    saveBinary(fileName + ".bin", numFrames, duration, numBones, tracks);

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

const std::string& Animation::getFileName() const {
    return fileName;
}

void Animation::getGlobalPoseAtTime(
      std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const {
    // Resize the outPoses vector if needed
    if(outPoses.size() != numBones) {
        outPoses.resize(numBones);
    }

    // Figure out the current frame index and next frame
    // (This assumes inTime is bounded by [0, AnimDuration])
    size_t frame = static_cast<size_t>(inTime / frameDuration);
    size_t nextFrame = frame + 1;
    // Calculate fractional value between frame and next frame
    float pct = inTime / frameDuration - frame;

    // Setup the pose for the root
    // Does the root have a track?
    if(tracks[0].size() > 0) {
        // Interpolate between the current frame's pose and the next frame
        BoneTransform interp =
              BoneTransform::interpolate(tracks[0][frame], tracks[0][nextFrame], pct);
        outPoses[0] = interp.toMatrix();
    } else {
        outPoses[0] = Matrix4::Identity;
    }

    const std::vector<Skeleton::Bone>& bones = inSkeleton->getBones();
    // Now setup the poses for the rest
    for(size_t bone = 1; bone < numBones; bone++) {
        Matrix4 localMat; // Defaults to identity
        if(tracks[bone].size() > 0) {
            BoneTransform interp =
                  BoneTransform::interpolate(tracks[bone][frame], tracks[bone][nextFrame], pct);
            localMat = interp.toMatrix();
        }
        outPoses[bone] = localMat * outPoses[bones[bone].parent];
    }
}

bool Animation::loadBinary(const std::string& fileName) {
    std::ifstream inFile(fileName, std::ios::in | std::ios::binary);
    if(inFile.is_open()) {
        AnimationBinHeader header;
        inFile.read(reinterpret_cast<char*>(&header), sizeof(header));

        char* sig = header.signature;
        if(sig[0] != 'G' || sig[1] != 'M' || sig[2] != 'S' || sig[3] != 'H'
              || header.version != binaryVersion) {
            return false;
        }

        numFrames = header.frames;
        duration = header.length;
        numBones = header.boneCount;
        frameDuration = duration / (numFrames - 1);

        tracks.resize(numBones);

        for(size_t i = 0; i < tracks.size(); i++) {
            unsigned boneTransformCount = 0;
            inFile.read(reinterpret_cast<char*>(&boneTransformCount), sizeof(unsigned));

            for(size_t j = 0; j < boneTransformCount; j++) {
                BoneTransform transform;
                inFile.read(reinterpret_cast<char*>(&transform), sizeof(BoneTransform));

                tracks[i].emplace_back(transform);
            }
        }
        return true;
    }

    return false;
}

void Animation::saveBinary(const std::string& fileName,
      size_t frames,
      float length,
      size_t numBones,
      std::vector<std::vector<BoneTransform>>& tracks) {
    AnimationBinHeader header;
    header.boneCount = numBones;
    header.frames = numFrames;
    header.length = duration;

    std::ofstream outFile(fileName, std::ios::out | std::ios::binary);
    if(outFile.is_open()) {
        outFile.write(reinterpret_cast<char*>(&header), sizeof(header));

        for(size_t i = 0; i < tracks.size(); i++) {
            // Write track count for bone
            unsigned bonesTranformsCount = static_cast<unsigned>(tracks[i].size());
            outFile.write(reinterpret_cast<char*>(&bonesTranformsCount), sizeof(unsigned));

            for(size_t j = 0; j < tracks[i].size(); j++) {
                outFile.write(reinterpret_cast<char*>(&tracks[i][j]), sizeof(BoneTransform));
            }
        }
    }
}