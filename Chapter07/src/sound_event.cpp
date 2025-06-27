#include "sound_event.h"

#include "audio_system.h"
#include "fmod_studio.hpp"

SoundEvent::SoundEvent() : system(nullptr), ID(0) {}

SoundEvent::SoundEvent(AudioSystem* system, unsigned int id) : system(system), ID(id) {}

bool SoundEvent::isValid() {
    return (system && system->getEventInstance(ID) != nullptr);
}

void SoundEvent::restart() {
    auto event = system ? system->getEventInstance(ID) : nullptr;
    if(event) {
        event->start();
    }
}

void SoundEvent::stop(bool allowFadeOut) {
    auto event = system ? system->getEventInstance(ID) : nullptr;
    if(event) {
        FMOD_STUDIO_STOP_MODE mode =
              allowFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;
        event->stop(mode);
    }
}

void SoundEvent::setPaused(bool pause) {
    auto event = system ? system->getEventInstance(ID) : nullptr;

    if(event) {
        event->setPaused(pause);
    }
}

void SoundEvent::setVolume(float value) {
    auto event = system ? system->getEventInstance(ID) : nullptr;
    if(event) {
        event->setVolume(value);
    }
}

void SoundEvent::setPitch(float value) {
    auto event = system ? system->getEventInstance(ID) : nullptr;
    if(event) {
        event->setPitch(value);
    }
}

void SoundEvent::setParameter(const std::string& name, float value) {
    auto event = system ? system->getEventInstance(ID) : nullptr;
    if(event) {
        event->setParameterByName(name.c_str(), value);
    }
}

bool SoundEvent::getPaused() const {
    bool retVal = false;
    auto event = system ? system->getEventInstance(ID) : nullptr;
    if(event) {
        event->getPaused(&retVal);
    }
    return retVal;
}

float SoundEvent::getVolume() const {
    float retVal = false;
    auto event = system ? system->getEventInstance(ID) : nullptr;
    if(event) {
        event->getVolume(&retVal);
    }
    return retVal;
}

float SoundEvent::getPitch() const {
    float retVal = false;
    auto event = system ? system->getEventInstance(ID) : nullptr;
    if(event) {
        event->getPitch(&retVal);
    }
    return retVal;
}

float SoundEvent::getParameter(const std::string& name) {
    float retVal = false;
    auto event = system ? system->getEventInstance(ID) : nullptr;
    if(event) {
        event->getParameterByName(name.c_str(), &retVal);
    }
    return retVal;
}

bool SoundEvent::is3D() const {
    bool retVal = false;
    auto event = system ? system->getEventInstance(ID) : nullptr;
    if(event) {
        // Get the event description
        FMOD::Studio::EventDescription* ed = nullptr;
        event->getDescription(&ed);
        if(ed) {
            ed->is3D(&retVal); // Is this 3D?
        }
    }
    return retVal;
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

void SoundEvent::set3DAttributes(const Matrix4& worldTrans, const Vector3& velocity) {
    auto event = system ? system->getEventInstance(ID) : nullptr;
    if(event) {
        FMOD_3D_ATTRIBUTES attr;
        // Set position, forward, up
        attr.position = VecToFMOD(worldTrans.GetTranslation());
        // In world transform, first row is forward
        attr.forward = VecToFMOD(worldTrans.GetXAxis());
        // Third row is up
        attr.up = VecToFMOD(worldTrans.GetZAxis());
        // Set velocity to zero (fix if using Doppler effect)
        attr.velocity = VecToFMOD(velocity);
        event->set3DAttributes(&attr);
    }
}
