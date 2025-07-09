#include "camera_component.h"

#include "actor.h"
#include "audio_system.h"
#include "game.h"
#include "renderer.h"

CameraComponent::CameraComponent(Actor* owner) : Component(owner) {}

void CameraComponent::setViewMatrix(const Matrix4& view) {
    // Pass view matrix to renderer and audio system
    Game* game = owner->getGame();
    game->getRenderer()->setViewMatrix(view);
    game->getAudioSystem()->setListener(view);
}