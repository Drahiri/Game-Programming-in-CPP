#ifndef HUD_H
#define HUD_H

#include "ui_screen.h"

#include <vector>

class Game;
class Shader;
class Texture;
class TargetComponent;

class HUD : public UIScreen {
public:
    // (Lower draw order corresponds with further back)
    HUD(Game* game);
    ~HUD();

    void update(float deltaTime) override;
    void draw(Shader* shader) override;

    void addTarget(TargetComponent* tc);
    void removeTarget(TargetComponent* tc);

protected:
    void updateCrosshair();
    void updateRadar();
    void updateArrow();

    // Textures for crosshair
    Texture* crosshair;
    Texture* crosshairEnemy;
    // Whether crosshair targets an enemy
    bool targetEnemy;
    // TargetComponents
    std::vector<TargetComponent*> targetComps;

    // Radar
    Texture* radar;
    Texture* blipUpTex;
    Texture* blipDownTex;
    Texture* radarArrow;
    // 2D offsets of blips relative to radar
    std::vector<Vector3> blips;
    // Adjust range of radar and radius
    float radarRange;
    float radarRadius;

    // Arrow for target arrow
    Texture* arrowTex;
    float arrowRot;
};

#endif