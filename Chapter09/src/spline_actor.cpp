#include "spline_actor.h"

#include "math.h"
#include "spline_camera.h"

SplineActor::SplineActor(Game* game) : Actor(game) {
    cameraComp = new SplineCamera(this);

    // Create a spline
    Spline path;
    path.controlPoints.emplace_back(Vector3::Zero);
    for(int i = 0; i < 5; i++) {
        if(i % 2 == 0) {
            path.controlPoints.emplace_back(Vector3(300.0f * (i + 1), 300.0f, 300.0f));
        } else {
            path.controlPoints.emplace_back(Vector3(300.0f * (i + 1), 0.0f, 0.0f));
        }
    }

    cameraComp->setSpline(path);
    cameraComp->setPaused(false);
}

void SplineActor::actorInput(const bool* keys) {}

void SplineActor::restartSpline() {
    cameraComp->restart();
}