#include "ball_move.h"

#include "actor.h"
#include "collision.h"
#include "game.h"
#include "phys_world.h"

BallMove::BallMove(Actor* owner) : MoveComponent(owner) {}

void BallMove::setPlayer(Actor* player) {
    this->player = player;
}

void BallMove::update(float deltaTime) {
    // Construct segment in direction of travel
    const float segmentLength = 30.0f;
    Vector3 start = owner->getPosition();
    Vector3 dir = owner->getForward();
    Vector3 end = start + dir * segmentLength;

    LineSegment ls(start, end);

    // Test segment vs world
    PhysWorld* phys = owner->getGame()->getPhysWorld();
    PhysWorld::CollisionInfo info;
    if(phys->segmentCast(ls, info)) {
        // If we collided, reflect the direction about the normal
        dir = Vector3::Reflect(dir, info.normal);
        owner->rotateToNewForward(dir);
    }

    // Base class update moves based on forward speed
    MoveComponent::update(deltaTime);
}
