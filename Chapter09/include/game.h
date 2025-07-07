#ifndef GAME_H
#define GAME_H

#include "math.h"
#include "sound_event.h"

#include <SDL3/SDL.h>
#include <string>
#include <vector>

class Actor;
class AudioSystem;
class FollowActor;
class FPSActor;
class OrbitActor;
class Renderer;
class SplineActor;
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

    // Time keeping
    Uint32 ticksCount;

    // Objects
    bool updatingActors;
    std::vector<Actor*> actors;
    std::vector<Actor*> pendingActors;

    // Game-specific code
    FPSActor* fpsActor;
    FollowActor* followActor;
    OrbitActor* orbitActor;
    SplineActor* splineActor;
    SpriteComponent* crosshair;
    Actor* startSphere;
    Actor* endSphere;
    SoundEvent musicEvent;
    SoundEvent reverbSnap;
    void changeCamera(int mode);
};

#endif