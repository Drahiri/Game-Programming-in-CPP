#include "fps_camera.h"

#include "actor.h"

FPSCamera::FPSCamera(Actor* owner) : CameraComponent(owner) {}

void FPSCamera::update(float deltaTime) {
    // Camera position is owner position
    Vector3 cameraPos = owner->getPosition();
    // Target position 100 units in front of owner
    Vector3 target = cameraPos + owner->getForward() * 100;
    // Up is just unit z
    Vector3 up = Vector3::UnitZ;

    // Create look at matrix, set as view
    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    setViewMatrix(view);
}