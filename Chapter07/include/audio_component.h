#ifndef AUDIO_COMPONENT_H
#define AUDIO_COMPONENT_H

#include "component.h"

#include <string>
#include <vector>

class Actor;
class SoundEvent;

class AudioComponent : public Component {
    AudioComponent(Actor* owner, int updateOrder = 200);
    ~AudioComponent();

    void update(float deltaTime) override;
    void onUpdateWorldTransform() override;

    SoundEvent playEvent(const std::string& name);
    void stopAllEvents();

private:
    std::vector<SoundEvent> events2D;
    std::vector<SoundEvent> events3D;
};

#endif