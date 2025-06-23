#include "audio_system.h"

#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <SDL3/SDL_log.h>
#include <vector>

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

    // Load the master banks (strings first)
    loadBank("assets/Master Bank.strings.bank");
    loadBank("assets/Master Bank.bank");

    return true;
}

void AudioSystem::shutdown() {
    if(system) {
        system->release();
    }
}

void AudioSystem::update(float deltaTime) {
    system->update();
}

void AudioSystem::loadBank(const std::string& name) {
    // Prevent double-loading
    if(banks.find(name) != banks.end()) {
        return;
    }

    // Try to load bank
    FMOD::Studio::Bank* bank = nullptr;
    FMOD_RESULT result = system->loadBankFile(name.c_str(), // File name of bank
          FMOD_STUDIO_LOAD_BANK_NORMAL,                     // Normal loading
          &bank                                             // Save pointer to bank
    );

    if(result != FMOD_OK) {
        SDL_Log("Error while loading bank '%s': %s", name.c_str(), FMOD_ErrorString(result));
        return;
    }

    const int maxPathLength = 512;
    // Add bank to map
    banks.emplace(name, bank);

    // Load all non-streaming sample data
    bank->loadSampleData();
    // Get the number of events in this bank
    int numEvents = 0;
    bank->getEventCount(&numEvents);

    if(numEvents > 0) {
        // Get list of event descriptions in this bank
        std::vector<FMOD::Studio::EventDescription*> eventDescs(numEvents);
        bank->getEventList(eventDescs.data(), numEvents, &numEvents);
        char eventName[maxPathLength];
        for(int i = 0; i < numEvents; i++) {
            FMOD::Studio::EventDescription* e = eventDescs[i];
            // Get the path of this event (like event:/Explosion2D)
            e->getPath(eventName, maxPathLength, nullptr);
            // Add to event map
            events.emplace(eventName, e);
        }
    }
}

void AudioSystem::unloadBank(const std::string& name) {
    // Ignore if not loaded
    auto iter = banks.find(name);
    if(iter == banks.end()) {
        return;
    }

    // First we nne to remove all events from this bank
    FMOD::Studio::Bank* bank = iter->second;
    int numEvents = 0;
    bank->getEventCount(&numEvents);
    if(numEvents > 0) {
        // Get event descriptions for this bank
        std::vector<FMOD::Studio::EventDescription*> eventDescs(numEvents);
        // Get list of evetns
        bank->getEventList(eventDescs.data(), numEvents, &numEvents);
        char eventName[512];
        for(int i = 0; i < numEvents; i++) {
            FMOD::Studio::EventDescription* e = eventDescs[i];
            // Get the path of this event
            e->getPath(eventName, 512, nullptr);
            // Remove this event
            auto eventi = events.find(eventName);
            if(eventi != events.end()) {
                events.erase(eventi);
            }
        }
    }

    // Unload sample data and bank
    bank->unloadSampleData();
    bank->unload();

    // Remove from banks map
    banks.erase(iter);
}

void AudioSystem::unloadAllBanks() {
    for(auto iter: banks) {
        // Unload the sample data, then the bank itself
        iter.second->unloadSampleData();
        iter.second->unload();
    }
    banks.clear();
    // No banks means no events
    events.clear();
}

void AudioSystem::playEvent(const std::string& name) {
    // Make sure event exists
    auto iter = events.find(name);
    if(iter != events.end()) {
        // Create instance of event
        FMOD::Studio::EventInstance* event = nullptr;
        iter->second->createInstance(&event);
        if(event) {
            // Start the event isntance
            event->start();
            // Release schedules destruction of the event
            // instance when it stops.
            // (Non-looping evnets automatically stop.)
            event->release();
        }
    }
}
