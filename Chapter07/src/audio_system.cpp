#include "audio_system.h"

#include "game.h"
#include "sound_event.h"

#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <SDL3/SDL_log.h>
#include <vector>

unsigned int AudioSystem::nextID = 0;

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
    // Find any stopped event instances
    std::vector<unsigned int> done;
    for(auto& iter: eventInstances) {
        FMOD::Studio::EventInstance* e = iter.second;

        // Get the state of this event
        FMOD_STUDIO_PLAYBACK_STATE state;
        e->getPlaybackState(&state);

        if(state == FMOD_STUDIO_PLAYBACK_STOPPED) {
            // Release the event and add id to done
            e->release();
            done.emplace_back(iter.first);
        }
    }

    // Remove done event instances from map
    for(auto id: done) {
        eventInstances.erase(id);
    }

    // Update FMOD
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

    // First we need to remove all events from this bank
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

SoundEvent AudioSystem::playEvent(const std::string& name) {
    unsigned int retID = 0;
    // Make sure event exists
    auto iter = events.find(name);
    if(iter != events.end()) {
        // Create instance of event
        FMOD::Studio::EventInstance* event = nullptr;
        iter->second->createInstance(&event);
        if(event) {
            // Start the event isntance
            event->start();
            // Get the next id, and add to map
            nextID++;
            retID = nextID;

            eventInstances.emplace(nextID, event);
        }
    }

    return SoundEvent(this, retID);
}

FMOD::Studio::EventInstance* AudioSystem::getEventInstance(unsigned int id) const {
    auto iter = eventInstances.find(id);
    if(iter == eventInstances.end()) {
        return nullptr;
    }

    return iter->second;
}

namespace {
FMOD_VECTOR VecToFMOD(const Vector3& in) {
    // Convert from our coordinates (+x forward, +y right, +z up)
    // to FMOD (+z forward, +x right, +y up)
    FMOD_VECTOR v;
    v.x = in.y;
    v.y = in.z;
    v.z = in.x;

    return v;
}
} // namespace

void AudioSystem::setListerner(const Matrix4& viewMatrix) {
    // Invert the view matrix to get the correct vectors
    Matrix4 invView = viewMatrix;
    invView.Invert();
    FMOD_3D_ATTRIBUTES listener;
    // Set position, forward, up
    listener.position = VecToFMOD(invView.GetTranslation());
    // In the inverted view, third row is forward
    listener.forward = VecToFMOD(invView.GetZAxis());
    // In the inverted view, second row is up
    listener.up = VecToFMOD(invView.GetYAxis());
    // Set velocity to zero (fix if using Doppler effect)
    listener.velocity = { 0.0f, 0.0f, 0.0f };
    // Send to FMOD (0 = only one listener)
    system->setListenerAttributes(0, &listener);
}
