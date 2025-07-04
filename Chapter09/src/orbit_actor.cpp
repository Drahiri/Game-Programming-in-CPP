#include "orbit_actor.h"

#include "audio_component.h"
#include "game.h"
#include "mesh_component.h"
#include "orbit_camera.h"
#include "renderer.h"

OrbitActor::OrbitActor(Game* game) : Actor(game) {
    cameraComp = new OrbitCamera(this);

    meshComp = new MeshComponent(this);
    meshComp->setMesh(game->getRenderer()->getMesh("assets/RacingCar.gpmesh"));
    setPosition(Vector3(0.0f, 0.0f, -100.0f));
}

void OrbitActor::actorInput(const bool* keyState) {
    // Mouse
    float x, y;
    Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);

    if(buttons & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)) {
        // Assume mouse movement is usually between -500 and 500
        const int maxMouseSpeed = 500;
        // Rotation/sec at maximum speed
        const float maxOrbitSpeed = Math::Pi * 8;
        float yawSpeed = 0.0f;

        if(x != 0) {
            // Convert to approximately [-1.0, 1.0]
            yawSpeed = x / maxMouseSpeed;
            // Multiply by rotation/sec
            yawSpeed *= maxOrbitSpeed;
        }
        cameraComp->setYawSpeed(-yawSpeed);

        // Compute pitch
        float pitchSpeed = 0.0f;
        if(y != 0) {
            // Convert to [-1.0, 1.0]
            pitchSpeed = y / maxMouseSpeed;
            pitchSpeed *= maxOrbitSpeed;
        }

        cameraComp->setPitchSpeed(pitchSpeed);
    }
}

void OrbitActor::setVisible(bool visible) {
    meshComp->setVisible(visible);
}