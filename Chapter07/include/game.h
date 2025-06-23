#ifndef GAME_H
#define GAME_H

#include "math.h"

#include <SDL3/SDL.h>
#include <string>
#include <vector>

class Actor;
class Renderer;
class CameraActor;
class AudioSystem;

class Game {
public:
    Game();

    bool initialize();
    void runLoop();
    void shutdown();

    void addActor(Actor* actor);
    void removeActor(Actor* actor);

    Renderer* getRenderer();

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
    CameraActor* cameraActor;
    // TODO: Uncomment after implementing sound
    /*
    SoundEvent musicEvent;
    SoundEvent reverbSnap;
    */
};

#endif