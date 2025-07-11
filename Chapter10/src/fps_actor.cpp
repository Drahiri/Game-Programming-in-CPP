#include "fps_actor.h"

#include "audio_component.h"
#include "audio_system.h"
#include "ball_actor.h"
#include "box_component.h"
#include "collision.h"
#include "fps_camera.h"
#include "game.h"
#include "mesh_component.h"
#include "move_component.h"
#include "plane_actor.h"
#include "renderer.h"

FPSActor::FPSActor(Game* game) : Actor(game) {
    moveComp = new MoveComponent(this);
    audioComp = new AudioComponent(this);
    cameraComp = new FPSCamera(this);

    lastFootstep = 0.0f;
    footstep = audioComp->playEvent("event:/Footstep");
    footstep.setPaused(true);

    fpsModel = new Actor(game);
    fpsModel->setScale(0.75f);
    meshComp = new MeshComponent(fpsModel);
    meshComp->setMesh(game->getRenderer()->getMesh("assets/Rifle.gpmesh"));

    // Add a box component
    boxComp = new BoxComponent(this);
    AABB myBox(Vector3(-25.0f, -25.0f, -87.5f), Vector3(25.0f, 25.0f, 87.5f));
    boxComp->setObjectBox(myBox);
    boxComp->setShouldRotate(false);
}

void FPSActor::actorInput(const bool* keyState) {
    float forwardSpeed = 0.0f;
    float strafeSpeed = 0.0f;
    // wasd movement
    if(keyState[SDL_SCANCODE_W]) {
        forwardSpeed += 400.0f;
    }
    if(keyState[SDL_SCANCODE_S]) {
        forwardSpeed -= 400.0f;
    }
    if(keyState[SDL_SCANCODE_A]) {
        strafeSpeed -= 400.0f;
    }
    if(keyState[SDL_SCANCODE_D]) {
        strafeSpeed += 400.0f;
    }

    moveComp->setForwardSpeed(forwardSpeed);
    moveComp->setStrafeSpeed(strafeSpeed);

    // Mouse
    float x, y;
    Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);

    // Assume mouse movement is usually between -500 and 500
    const int maxMouseSpeed = 500;
    // Rotation/sec at maximum speed
    const float maxAngularSpeed = Math::Pi * 8;
    float angularSpeed = 0.0f;

    if(x != 0) {
        // Convert to approximately [-1.0, 1.0]
        angularSpeed = x / maxMouseSpeed;
        // Multiply by rotation/sec
        angularSpeed *= maxAngularSpeed;
    }

    moveComp->setAngularSpeed(angularSpeed);

    // Compute pitch
    const float maxPitchSpeed = Math::Pi * 8;
    float pitchSpeed = 0.0f;
    if(y != 0) {
        // Convert to [-1.0, 1.0]
        pitchSpeed = y / maxMouseSpeed;
        pitchSpeed *= maxPitchSpeed;
    }

    cameraComp->setPitchSpeed(pitchSpeed);
}

void FPSActor::updateActor(float deltaTime) {
    Actor::updateActor(deltaTime);

    fixCollisions();

    // Play the footstep if we're moving and havent't recently
    lastFootstep -= deltaTime;
    if(!Math::NearZero(moveComp->getForwardSpeed()) && lastFootstep <= 0.0f) {
        footstep.setPaused(false);
        footstep.restart();
        lastFootstep = 0.5f;
    }

    // Update position of FPS model relative to actor position
    const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
    Vector3 modelPos = getPosition();
    modelPos += getForward() * modelOffset.x;
    modelPos += getRight() * modelOffset.y;
    modelPos.z += modelOffset.z;
    fpsModel->setPosition(modelPos);

    // Initialize rotation to actor rotation
    Quaternion q = getRotation();

    // Rotate by pitch from camera
    q = Quaternion::Concatenate(q, Quaternion(getRight(), cameraComp->getPitch()));
    fpsModel->setRotation(q);
}

void FPSActor::setFootstepSurface(float value) {
    // Pause here because the way parameter is setup for FMOD
    // changing it will play a footstep
    footstep.setPaused(true);
    footstep.setParameter("Surface", value);
}

void FPSActor::setVisible(bool visible) {
    meshComp->setVisible(visible);
}

void FPSActor::shoot() {
    // Get start point (in center of screen on near plane)
    Vector3 screenPoint(0.0f, 0.0f, 0.0f);
    Vector3 start = getGame()->getRenderer()->unproject(screenPoint);
    // Get end point (in center of screen, between near and far)
    screenPoint.z = 0.9f;
    Vector3 end = getGame()->getRenderer()->unproject(screenPoint);
    // Get direction vector
    Vector3 dir = end - start;
    dir.Normalize();
    // Spawn a ball
    BallActor* ball = new BallActor(getGame());
    ball->setPlayer(this);
    ball->setPosition(start + dir * 20.0f);
    // Rotate the ball to face new direction
    ball->rotateToNewForward(dir);
    // Play shooting event
    audioComp->playEvent("event:/Shot");
}

void FPSActor::fixCollisions() {
    // Need to recompute my world transform to update world box
    computeWorldTransform();

    const AABB& playerBox = boxComp->getWorldBox();
    Vector3 pos = getPosition();

    auto& planes = getGame()->getPlanes();
    for(auto pa: planes) {
        // Do we collide with this PlaneActor?
        const AABB& planeBox = pa->getBox()->getWorldBox();
        if(intersect(playerBox, planeBox)) {
            // Calculate all our differences
            float dx1 = planeBox.min.x - playerBox.max.x;
            float dx2 = planeBox.max.x - playerBox.min.x;
            float dy1 = planeBox.min.y - playerBox.max.y;
            float dy2 = planeBox.max.y - playerBox.min.y;
            float dz1 = planeBox.min.z - playerBox.max.z;
            float dz2 = planeBox.max.z - playerBox.min.z;

            // Set dx to whickever of dx1/dx2 have a lower abs
            float dx = (Math::Abs(dx1) < Math::Abs(dx2) ? dx1 : dx2);
            // Ditto for dy
            float dy = (Math::Abs(dy1) < Math::Abs(dy2) ? dy1 : dy2);
            // Ditto for dz
            float dz = (Math::Abs(dz1) < Math::Abs(dz2) ? dz1 : dz2);

            // Whickever is closes, adjust x/y position
            if(Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz)) {
                pos.x += dx;
            } else if(Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz)) {
                pos.y += dy;
            } else {
                pos.z += dz;
            }

            // Need to set positions and update box component
            setPosition(pos);
            boxComp->onUpdateWorldTransform();
        }
    }
}