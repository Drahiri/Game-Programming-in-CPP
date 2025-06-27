#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include "math.h"

#include <string>
#include <unordered_map>

// FMOD forward declaration
namespace FMOD {
class System;

namespace Studio {
class Bank;
class Bus;
class EventDescription;
class EventInstance;
class System;
} // namespace Studio
} // namespace FMOD

class Game;
class SoundEvent;

class AudioSystem {
public:
    AudioSystem(Game* game);
    ~AudioSystem();

    bool initialize();
    void shutdown();
    void update(float deltaTime);

    void loadBank(const std::string& name);
    void unloadBank(const std::string& name);
    void unloadAllBanks();

    SoundEvent playEvent(const std::string& name);
    void setListener(const Matrix4& viewMatrix, const Vector3& velocity);

    float getBusVolume(const std::string& name) const;
    void setBusVolume(const std::string& name, float value);

    bool getBusPaused(const std::string& name) const;
    void setBusPaused(const std::string& name, bool paused);

protected:
    friend SoundEvent;
    FMOD::Studio::EventInstance* getEventInstance(unsigned int id) const;

private:
    Game* game;

    // FMOD studio system
    FMOD::Studio::System* system;

    // FMOD Low-lewel system (in case needed)
    FMOD::System* lowLevelSystem;

    // Map of loaded banks
    std::unordered_map<std::string, FMOD::Studio::Bank*> banks;

    // Map of event name to EventDescription
    std::unordered_map<std::string, FMOD::Studio::EventDescription*> events;

    // Map of event instances
    std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> eventInstances;
    static unsigned int nextID;

    // Map of buses
    std::unordered_map<std::string, FMOD::Studio::Bus*> buses;
};

#endif