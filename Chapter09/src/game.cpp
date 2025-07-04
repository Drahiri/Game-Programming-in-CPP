#include "game.h"

#include "actor.h"
#include "audio_component.h"
#include "audio_system.h"
#include "fps_actor.h"
#include "mesh_component.h"
#include "plane_actor.h"
#include "renderer.h"
#include "sprite_component.h"

#include <algorithm>

const int windowWidth = 1024;
const int windowHeight = 768;

Game::Game() : isRunning(true), ticksCount(0), updatingActors(true), renderer(nullptr) {}

bool Game::initialize() {
    // Initialize SDL
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Create the renderer
    renderer = new Renderer(this);
    if(!renderer->initialize(windowWidth, windowHeight)) {
        SDL_Log("Failed to initialize renderer.");
        delete renderer;
        renderer = nullptr;
        return false;
    }

    // Create audio system
    audioSystem = new AudioSystem(this);
    if(!audioSystem->initialize()) {
        SDL_Log("Failed to initialize audio system.");
        delete audioSystem;
        audioSystem = nullptr;
        return false;
    }

    loadData();

    return true;
}

void Game::runLoop() {
    while(isRunning) {
        processInput();
        updateGame();
        generateOutput();
    }
}

void Game::shutdown() {
    // Because ~Actor calls RemoveActor, use a different style loop
    unloadData();

    if(renderer) {
        renderer->shutdown();
    }
    if(audioSystem) {
        audioSystem->shutdown();
    }

    SDL_Quit();
}

void Game::addActor(Actor* actor) {
    // If updating actors, need to add to pending
    if(updatingActors) {
        pendingActors.emplace_back(actor);
    } else {
        actors.emplace_back(actor);
    }
}

void Game::removeActor(Actor* actor) {
    // Is it in actors?
    auto iter = std::find(actors.begin(), actors.end(), actor);
    if(iter != actors.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, actors.end() - 1);
        actors.pop_back();
    }

    // Is it in pending actors?
    iter = std::find(pendingActors.begin(), pendingActors.end(), actor);
    if(iter != pendingActors.end()) {
        std::iter_swap(iter, pendingActors.end() - 1);
        pendingActors.pop_back();
    }
}

Renderer* Game::getRenderer() {
    return renderer;
}

void Game::processInput() {
    // Process SDL events
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;

        // This fires when a key's initially pressed
        case SDL_EVENT_KEY_DOWN:
            if(!event.key.repeat) {
                handleKeyPress(event.key.key);
            }

        default:
            break;
        }
    }

    // Process keyboard state
    const bool* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]) {
        isRunning = false;
    }

    updatingActors = true;
    for(auto actor: actors) {
        actor->processInput(state);
    }
    updatingActors = false;
}

AudioSystem* Game::getAudioSystem() {
    return audioSystem;
}

void Game::handleKeyPress(int key) {
    switch(key) {
    case '-': { // Reduce master volume
        float volume = audioSystem->getBusVolume("bus:/");
        volume = Math::Max(0.0f, volume - 0.1f);
        audioSystem->setBusVolume("bus:/", volume);
        break;
    }

    case '=': { // Increase master volume
        float volume = audioSystem->getBusVolume("bus:/");
        volume = Math::Max(0.0f, volume + 0.1f);
        audioSystem->setBusVolume("bus:/", volume);
        break;
    }

    default:
        break;
    }
}

void Game::updateGame() {
    // Limit frame rate
    while(SDL_GetTicks() < ticksCount + 16) {}

    // Delta time is the difference in ticks from last frame
    // (converted to seconds)
    Uint32 currectTick = SDL_GetTicks();
    float deltaTime = (currectTick - ticksCount) / 1000.0f;
    ticksCount = currectTick;

    // Clamp maximum delta time value
    if(deltaTime > 0.05f) {
        deltaTime = 0.05;
    }

    // Update all actors
    updatingActors = true;

    for(auto actor: actors) {
        actor->update(deltaTime);
    }

    updatingActors = false;

    // Move pending actors to actors
    for(auto pending: pendingActors) {
        pending->computeWorldTransform();
        actors.emplace_back(pending);
    }

    pendingActors.clear();

    // Add any dead actors to a temp vector
    std::vector<Actor*> deadActors;
    for(auto actor: actors) {
        if(actor->getState() == Actor::State::Dead) {
            deadActors.emplace_back(actor);
        }
    }

    // Delete dead actors (whick remove them from actors or pendingActors)
    for(auto actor: deadActors) {
        delete actor;
    }

    // Update audio system
    audioSystem->update(deltaTime);
}

void Game::generateOutput() {
    renderer->draw();
}

void Game::loadData() {
    // Create actors
    // Cube
    Actor* a = new Actor(this);
    a->setPosition(Vector3(200.0f, 75.0f, 0.0f));
    a->setScale(100.0f);
    Quaternion q(Vector3::UnitY, -Math::PiOver2);
    q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
    a->setRotation(q);
    MeshComponent* mc = new MeshComponent(a);
    mc->setMesh(renderer->getMesh("assets/Cube.gpmesh"));

    // Sphere
    a = new Actor(this);
    a->setPosition(Vector3(200.0f, -75.0f, 0.0f));
    a->setScale(3.0f);
    mc = new MeshComponent(a);
    mc->setMesh(renderer->getMesh("assets/Sphere.gpmesh"));

    // Setup floor
    const float start = -1250.0f;
    const float size = 250.0f;
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            a = new PlaneActor(this);
            a->setPosition(Vector3(start + i * size, start + j * size, -100.0f));
        }
    }

    // Left/right walls
    q = Quaternion(Vector3::UnitX, Math::PiOver2);
    for(int i = 0; i < 10; i++) {
        a = new PlaneActor(this);
        a->setPosition(Vector3(start + i * size, start - size, 0.0f));
        a->setRotation(q);

        a = new PlaneActor(this);
        a->setPosition(Vector3(start + i * size, -start + size, 0.0f));
        a->setRotation(q);
    }

    // Forward/back walls
    q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
    for(int i = 0; i < 10; i++) {
        a = new PlaneActor(this);
        a->setPosition(Vector3(start - size, start + i * size, 0.0f));
        a->setRotation(q);

        a = new PlaneActor(this);
        a->setPosition(Vector3(-start + size, start + i * size, 0.0f));
        a->setRotation(q);
    }

    // Setup lights
    renderer->setAmbientLight(Vector3(0.2, 0.2, 0.2));
    DirectionalLight& dir = renderer->getDirectionalLight();
    dir.direction = Vector3(0, -0.707, -0.707);
    dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
    dir.specColor = Vector3(0.8f, 0.8f, 0.8f);

    // // UI elements
    a = new Actor(this);
    a->setPosition(Vector3(-350.0f, -350.0f, 0.0f));
    SpriteComponent* sc = new SpriteComponent(a);
    sc->setTexture(renderer->getTexture("assets/HealthBar.png"));

    a = new Actor(this);
    a->setPosition(Vector3(-390.0f, 275.0f, 0.0f));
    a->setScale(0.75f);
    sc = new SpriteComponent(a);
    sc->setTexture(renderer->getTexture("assets/Radar.png"));

    a = new Actor(this);
    a->setScale(2.0f);
    crosshair = new SpriteComponent(a);
    crosshair->setTexture(renderer->getTexture("assets/Crosshair.png"));

    // Start music
    musicEvent = audioSystem->playEvent("event:/Music");

    fpsActor = new FPSActor(this);
}

void Game::unloadData() {
    // Delete actors
    while(!actors.empty()) {
        delete actors.back();
    }
}