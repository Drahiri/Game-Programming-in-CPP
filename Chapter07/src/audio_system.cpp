#include "audio_system.h"

#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <SDL3/SDL_log.h>

AudioSystem::AudioSystem(Game* game) : game(game), system(nullptr), lowLevelSystem(nullptr) {}

AudioSystem::~AudioSystem() {}

bool AudioSystem::initialize() {
    FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR, // Log only errors
          FMOD_DEBUG_MODE_TTY                      // Output to stdout
    );

    FMOD_RESULT result = FMOD::Studio::System::create(&system);
    if(result != FMOD_OK) {
        SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
        return false;
    }

    result = system->initialize(512, // Max number of concurrent channels
          FMOD_STUDIO_INIT_NORMAL,   // Use default settings
          FMOD_INIT_NORMAL,          // Use default settings
          nullptr                    // Usually null
    );

    if(result != FMOD_OK) {
        SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
        return false;
    }

    system->getCoreSystem(&lowLevelSystem);

    return true;
}

void AudioSystem::shutdown() {
    system->release();
}

void AudioSystem::update(float deltaTime) {
    system->update();
}