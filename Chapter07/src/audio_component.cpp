#include "audio_component.h"

#include "actor.h"
#include "audio_system.h"
#include "camera_actor.h"
#include "game.h"
#include "sound_event.h"

AudioComponent::AudioComponent(Actor* owner, int updateOrder) : Component(owner, updateOrder) {};

AudioComponent::~AudioComponent() {
    stopAllEvents();
}

SoundEvent AudioComponent::playEvent(const std::string& name) {
    SoundEvent e = owner->getGame()->getAudioSystem()->playEvent(name);

    // is this 2D or 3D?
    if(e.is3D()) {
        events3D.emplace_back(e);
        // Set initial 3D attributes
        e.set3DAttributes(getVirtualTransform(), owner->getVelocity());
    } else {
        events2D.emplace_back(e);
    }

    return e;
}

void AudioComponent::update(float deltaTime) {
    Component::update(deltaTime);

    updateVirtualTransform();
    // Remove invalid 2D events
    auto iter = events2D.begin();
    while(iter != events2D.end()) {
        if(!iter->isValid()) {
            iter = events2D.erase(iter);
        } else {
            ++iter;
        }
    }

    // Remove invalid 3D events
    iter = events3D.begin();
    while(iter != events3D.end()) {
        if(!iter->isValid()) {
            iter = events3D.erase(iter);
        } else {
            ++iter;
        }
    }
}

void AudioComponent::onUpdateWorldTransform() {
    Matrix4 world = getVirtualTransform();
    for(auto& event: events3D) {
        if(event.isValid()) {
            event.set3DAttributes(world, owner->getVelocity());
        }
    }
}

void AudioComponent::stopAllEvents() {
    // Stop all sounds
    for(auto& event: events2D) {
        event.stop(true);
    }

    for(auto& event: events3D) {
        event.stop();
    }

    // Clear events
    events2D.clear();
    events3D.clear();
}

void AudioComponent::updateVirtualTransform() {
    if(owner->getGame()->getCameraActor() == nullptr) {
        return;
    }

    Vector3 playerToSound =
          owner->getPosition() - owner->getGame()->getCameraActor()->getPosition();
    Vector3 cameraToSound =
          owner->getPosition() - owner->getGame()->getCameraActor()->getCameraPosition();
    cameraToSound.Normalize();

    Vector3 virtualPosition = playerToSound.Length() * cameraToSound;

    virtualTransform = Matrix4::CreateScale(owner->getScale());
    virtualTransform *= Matrix4::CreateFromQuaternion(owner->getRotation());
    virtualTransform *= Matrix4::CreateTranslation(
          virtualPosition + owner->getGame()->getCameraActor()->getCameraPosition());

    onUpdateWorldTransform();
}

const Matrix4& AudioComponent::getVirtualTransform() const {
    return virtualTransform;
}