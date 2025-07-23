#ifndef SOUND_EVENT_H
#define SOUND_EVENT_H

#include "math.h"

#include <string>
class AudioSystem;

class SoundEvent {
public:
    SoundEvent();
    // Returns true if associated FMOD event instance exists
    bool isValid();

    // Restart event from beginning
    void restart();
    // Stop this event
    void stop(bool allowFadeOut = true);
    // Setters
    void setPaused(bool pause);
    void setVolume(float value);
    void setPitch(float value);
    void setParameter(const std::string& name, float value);
    // Getters
    bool getPaused() const;
    float getVolume() const;
    float getPitch() const;
    float getParameter(const std::string& name);
    // Positionals
    bool is3D() const;
    void set3DAttributes(const Matrix4& worldTrans);

protected:
    // Make this constructor protected and AudioSystem a friend
    // so that only AudioSystem can access this constructor.
    friend AudioSystem;
    SoundEvent(AudioSystem* system, unsigned int id);

private:
    AudioSystem* system;
    unsigned int ID;
};

#endif