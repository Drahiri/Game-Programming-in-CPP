#ifndef GAME_H
#define GAME_H

#include "math.h"
#include "sound_event.h"

#include <SDL3/SDL.h>
#include <string>
#include <vector>

class Actor;
class AudioSystem;
class FPSActor;
class PhysWorld;
class PlaneActor;
class Renderer;
class SpriteComponent;

class Game {
public:
    Game();

    bool initialize();
    void runLoop();
    void shutdown();

    void addActor(Actor* actor);
    void removeActor(Actor* actor);

    Renderer* getRenderer();
    AudioSystem* getAudioSystem();
    PhysWorld* getPhysWorld();

    // Game specific
    void addPlane(PlaneActor* plane);
    void removePlane(PlaneActor* plane);
    std::vector<PlaneActor*>& getPlanes();

private:
    // Helper functions for the game loop
    void processInput();
    void handleKeyPress(int key);
    void updateGame();
    void generateOutput();

    void loadData();
    void unloadData();

    bool isRunning;

    Renderer* renderer;
    AudioSystem* audioSystem;
    PhysWorld* physWorld;

    // Time keeping
    Uint32 ticksCount;

    // Objects
    bool updatingActors;
    std::vector<Actor*> actors;
    std::vector<Actor*> pendingActors;

    // Game-specific code
    std::vector<PlaneActor*> planes;
    FPSActor* fpsActor;
    SpriteComponent* crosshair;
    SoundEvent musicEvent;
};

#endif