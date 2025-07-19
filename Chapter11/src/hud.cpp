#include "hud.h"

#include "collision.h"
#include "game.h"
#include "phys_world.h"
#include "renderer.h"
#include "target_component.h"

#include <algorithm>

HUD::HUD(Game* game) : UIScreen(game), targetEnemy(false) {
    Renderer* r = game->getRenderer();
    crosshair = r->getTexture("assets/Crosshair.png");
    crosshairEnemy = r->getTexture("assets/CrosshairRed.png");
}

HUD::~HUD() {}

void HUD::update(float deltaTime) {
    updateCrosshair();
}

void HUD::draw(Shader* shader) {
    Texture* cross = targetEnemy ? crosshairEnemy : crosshair;
    drawTexture(shader, cross, Vector2::Zero, 2.0f);
}

void HUD::addTarget(TargetComponent* tc) {
    targetComps.emplace_back(tc);
}

void HUD::removeTarget(TargetComponent* tc) {
    auto iter = std::find(targetComps.begin(), targetComps.end(), tc);
    if(iter != targetComps.end()) {
        targetComps.erase(iter);
    }
}

void HUD::updateCrosshair() {
    // Reset to regular cursor
    targetEnemy = false;
    // Make a line segment
    const float cAimDist = 5000.0f;
    Vector3 start, dir;
    game->getRenderer()->getScreenDirection(start, dir);
    LineSegment l(start, start + dir * cAimDist);
    // Segment cast
    PhysWorld::CollisionInfo info;
    if(game->getPhysWorld()->segmentCast(l, info)) {
        // Check if this actor has a target component
        for(auto tc: targetComps) {
            if(tc->getOwner() == info.actor) {
                targetEnemy = true;
                break;
            }
        }
    }
}
