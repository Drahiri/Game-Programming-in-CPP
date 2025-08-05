#include "game.h"

#include "actor.h"
#include "animation.h"
#include "audio_component.h"
#include "audio_system.h"
#include "follow_actor.h"
#include "font.h"
#include "hud.h"
#include "level_loader.h"
#include "mesh_component.h"
#include "pause_menu.h"
#include "phys_world.h"
#include "plane_actor.h"
#include "point_light_component.h"
#include "renderer.h"
#include "skeleton.h"
#include "sprite_component.h"
#include "target_actor.h"
#include "ui_screen.h"

#include <algorithm>
#include <fstream>
#include <rapidjson/document.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <sstream>

const int windowWidth = 1024;
const int windowHeight = 768;

Game::Game() :
    gameState(GameState::Gameplay),
    ticksCount(0),
    updatingActors(true),
    renderer(nullptr),
    physWorld(nullptr),
    hud(nullptr) {}

bool Game::initialize() {
    // Initialize SDL
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Initialize SDL_ttf
    if(!TTF_Init()) {
        SDL_Log("Unable to initialize SDL_ttf: %s", SDL_GetError());
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

    // Create the physics world
    physWorld = new PhysWorld(this);

    loadData();

    return true;
}

void Game::runLoop() {
    while(gameState != GameState::Quit) {
        processInput();
        updateGame();
        generateOutput();
    }
}

void Game::shutdown() {
    // Because ~Actor calls RemoveActor, use a different style loop
    unloadData();

    TTF_Quit();
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

FollowActor* Game::getPlayer() const {
    return followActor;
}

void Game::processInput() {
    // Process SDL events
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_EVENT_QUIT:
            gameState = GameState::Quit;
            break;

        // This fires when a key's initially pressed
        case SDL_EVENT_KEY_DOWN:
            if(!event.key.repeat) {
                if(gameState == GameState::Gameplay) {
                    handleKeyPress(event.key.key);
                } else if(!uiStack.empty()) {
                    uiStack.back()->handleKeyPress(event.key.key);
                }
            }
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if(gameState == GameState::Gameplay) {
                handleKeyPress(event.button.button);
            } else if(!uiStack.empty()) {
                uiStack.back()->handleKeyPress(event.button.button);
            }
            break;

        default:
            break;
        }
    }

    // Process keyboard state
    const bool* state = SDL_GetKeyboardState(NULL);
    if(gameState == GameState::Gameplay) {
        updatingActors = true;
        for(auto actor: actors) {
            actor->processInput(state);
        }
        updatingActors = false;
    } else if(!uiStack.empty()) {
        uiStack.back()->processInput(state);
    }
}

Renderer* Game::getRenderer() {
    return renderer;
}

AudioSystem* Game::getAudioSystem() {
    return audioSystem;
}

PhysWorld* Game::getPhysWorld() {
    return physWorld;
}

Font* Game::getFont(const std::string& fileName) {
    auto iter = fonts.find(fileName);
    if(iter != fonts.end()) {
        return iter->second;
    } else {
        Font* font = new Font(this);
        if(font->load(fileName)) {
            fonts.emplace(fileName, font);
        } else {
            font->unload();
            delete font;
            font = nullptr;
        }
        return font;
    }
}

HUD* Game::getHUD() const {
    return hud;
}

void Game::pushUI(UIScreen* screen) {
    uiStack.emplace_back(screen);
}

const std::vector<UIScreen*>& Game::getUIStack() {
    return uiStack;
}

Game::GameState Game::getState() const {
    return gameState;
}

void Game::setState(GameState state) {
    gameState = state;
}

void Game::loadText(const std::string& fileName) {
    // Clear the existing map if already loaded
    textMap.clear();
    // Try to open the file
    std::ifstream file(fileName);
    if(!file.is_open()) {
        SDL_Log("Text file %s not found", fileName.c_str());
        return;
    }

    // Read the entire file to a string stream
    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string content = fileStream.str();
    // Open this file in rapidJSON
    rapidjson::StringStream jsonStr(content.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);
    if(!doc.IsObject()) {
        SDL_Log("Text file %s is not valid JSON", fileName.c_str());
        return;
    }
    // Parse the text map
    const rapidjson::Value& actions = doc["TextMap"];
    for(rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
          itr != actions.MemberEnd();
          ++itr) {
        if(itr->name.IsString() && itr->value.IsString()) {
            textMap.emplace(itr->name.GetString(), itr->value.GetString());
        }
    }
}

const std::string& Game::getText(const std::string& key) {
    static std::string errorMsg("** KEY NOT FOUND**");
    // Find this text in the map, if it exists
    auto iter = textMap.find(key);
    if(iter != textMap.end()) {
        return iter->second;
    } else {
        return errorMsg;
    }
}

Skeleton* Game::getSkeleton(const std::string& fileName) {
    auto iter = skeletons.find(fileName);
    if(iter != skeletons.end()) {
        return iter->second;
    } else {
        Skeleton* sk = new Skeleton();
        if(sk->load(fileName)) {
            skeletons.emplace(fileName, sk);
        } else {
            delete sk;
            sk = nullptr;
        }
        return sk;
    }
}

Animation* Game::getAnimation(const std::string& fileName) {
    auto iter = animations.find(fileName);
    if(iter != animations.end()) {
        return iter->second;
    } else {
        Animation* anim = new Animation();
        if(anim->load(fileName)) {
            animations.emplace(fileName, anim);
        } else {
            delete anim;
            anim = nullptr;
        }
        return anim;
    }
}

void Game::addPlane(PlaneActor* plane) {
    planes.emplace_back(plane);
}

void Game::removePlane(PlaneActor* plane) {
    auto iter = std::find(planes.begin(), planes.end(), plane);
    planes.erase(iter);
}

std::vector<PlaneActor*>& Game::getPlanes() {
    return planes;
}

void Game::handleKeyPress(int key) {
    switch(key) {
    case SDLK_ESCAPE:
        new PauseMenu(this);
        break;
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

    case '1':
        loadText("assets/English.gptext");
        break;

    case '2':
        loadText("assets/Russian.gptext");
        break;
    case SDL_BUTTON_LEFT: {
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

    if(gameState == GameState::Gameplay) {
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
    }

    // Update UIScreens
    for(auto ui: uiStack) {
        if(ui->getState() == UIScreen::UIState::Active) {
            ui->update(deltaTime);
        }
    }
    // Delete any UIScreens that are closed
    auto iter = uiStack.begin();
    while(iter != uiStack.end()) {
        if((*iter)->getState() == UIScreen::UIState::Closing) {
            delete *iter;
            iter = uiStack.erase(iter);
        } else {
            ++iter;
        }
    }

    // Update audio system
    audioSystem->update(deltaTime);
}

void Game::generateOutput() {
    renderer->draw();
}

void Game::loadData() {
    loadText("assets/English.gptext");

    // UI elements
    hud = new HUD(this);

    // Setup lights
    LevelLoader::loadLevel(this, "assets/Level3.gplevel");

    // Start music
    musicEvent = audioSystem->playEvent("event:/Music");

    followActor = new FollowActor(this);
}

void Game::unloadData() {
    // Delete actors
    while(!actors.empty()) {
        delete actors.back();
    }

    // Delete ui
    while(!uiStack.empty()) {
        delete uiStack.back();
        uiStack.pop_back();
    }

    if(renderer) {
        renderer->shutdown();
    }
    if(audioSystem) {
        audioSystem->shutdown();
    }

    if(physWorld) {
        delete physWorld;
    }

    while(!fonts.empty()) {
        auto iter = fonts.begin();
        delete iter->second;
        fonts.erase(iter);
    }

    // Unload skeletons
    for(auto s: skeletons) {
        delete s.second;
    }

    // Unload animations
    for(auto a: animations) {
        delete a.second;
    }
}