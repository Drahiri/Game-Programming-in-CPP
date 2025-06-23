#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

// FMOD forward declaration
namespace FMOD {
class System;

namespace Studio {
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

private:
    Game* game;
    // FMOD studio system
    FMOD::Studio::System* system;

    // FMOD Low-lewel system (in case needed)
    FMOD::System* lowLevelSystem;
};

#endif