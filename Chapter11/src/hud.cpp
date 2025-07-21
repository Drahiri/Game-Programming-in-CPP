#include "hud.h"

#include "collision.h"
#include "fps_actor.h"
#include "game.h"
#include "phys_world.h"
#include "renderer.h"
#include "target_component.h"

#include <algorithm>

HUD::HUD(Game* game) : UIScreen(game), targetEnemy(false), radarRange(2000.0f), radarRadius(92.0f) {
    Renderer* r = game->getRenderer();
    crosshair = r->getTexture("assets/Crosshair.png");
    crosshairEnemy = r->getTexture("assets/CrosshairRed.png");
    radar = r->getTexture("assets/Radar.png");
    blipTex = r->getTexture("assets/Blip.png");
    radarArrow = r->getTexture("assets/RadarArrow.png");
}

HUD::~HUD() {}

void HUD::update(float deltaTime) {
    updateCrosshair();
    updateRadar();
}

void HUD::draw(Shader* shader) {
    Texture* cross = targetEnemy ? crosshairEnemy : crosshair;
    drawTexture(shader, cross, Vector2::Zero, 2.0f);

    // Drawing radar
    const Vector2 cRadarPos(-350.0f, 250.0f);
    drawTexture(shader, radar, cRadarPos, 1.0f);
    // Blips
    for(const Vector2& blip: blips) {
        drawTexture(shader, blipTex, cRadarPos + blip, 1.0f);
    }
    drawTexture(shader, radarArrow, cRadarPos);
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
    // Move start point by 4 unit in dir to avoid collisions with player
    start += dir * 4;
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

void HUD::updateRadar() {
    // Clear blip positions from last frame
    blips.clear();

    // Convert player position to radar coordinates (x-y plane)
    Vector3 playerPos = game->getPlayer()->getPosition();
    Vector2 playerPos2D(playerPos.y, playerPos.x);
    // Ditto for player forward
    Vector3 playerForward = game->getPlayer()->getForward();
    Vector2 playerForward2D(playerForward.x, playerForward.y);

    // Use atan2 to get rotation of radar
    float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
    // Make a 2D rotation matrix
    Matrix3 rotMat = Matrix3::CreateRotation(angle);

    // Ge position of blips
    for(auto tc: targetComps) {
        Vector3 targetPos = tc->getOwner()->getPosition();
        Vector2 actorPos2D(targetPos.y, targetPos.x);

        // Calculate vector between player and target
        Vector2 playerToTarget = actorPos2D - playerPos2D;

        // See if within range
        if(playerToTarget.LengthSq() <= (radarRange * radarRange)) {
            // Convert playerToTarget into an offset from
            // the center of the on-screen radar
            Vector2 blipPos = playerToTarget;
            blipPos *= radarRadius / radarRange;
            // Rotate blipPos
            blipPos = Vector2::Transform(blipPos, rotMat);
            blips.emplace_back(blipPos);
        }
    }
}
