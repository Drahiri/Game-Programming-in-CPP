#ifndef GAME_H
#define GAME_H

#include "math.h"
#include "sound_event.h"

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

class Actor;
class AudioSystem;
class Font;
class FPSActor;
class PhysWorld;
class PlaneActor;
class Renderer;
class SpriteComponent;
class UIScreen;

class Game {
public:
    Game();

    enum class GameState {
        Gameplay,
        Paused,
        Quit
    };

    bool initialize();
    void runLoop();
    void shutdown();

    void addActor(Actor* actor);
    void removeActor(Actor* actor);

    Renderer* getRenderer();
    AudioSystem* getAudioSystem();
    PhysWorld* getPhysWorld();
    Font* getFont(const std::string& fileName);

    void pushUI(UIScreen* screen);
    const std::vector<UIScreen*>& getUIStack();

    GameState getState() const;
    void setState(GameState state);

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

    GameState gameState;

    Renderer* renderer;
    AudioSystem* audioSystem;
    PhysWorld* physWorld;
    std::unordered_map<std::string, Font*> fonts;

    // Time keeping
    Uint32 ticksCount;

    // Objects
    bool updatingActors;
    std::vector<Actor*> actors;
    std::vector<Actor*> pendingActors;

    std::vector<UIScreen*> uiStack;

    // Game-specific code
    std::vector<PlaneActor*> planes;
    FPSActor* fpsActor;
    SpriteComponent* crosshair;
    SoundEvent musicEvent;
};

#endif