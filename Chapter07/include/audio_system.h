#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include <string>
#include <unordered_map>

// FMOD forward declaration
namespace FMOD {
class System;

namespace Studio {
class Bank;
class EventDescription;
class System;
} // namespace Studio
} // namespace FMOD

class Game;

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
};

#endif