#include "game.h"

#include "actor.h"
#include "asteroid.h"
#include "bg_sprite_component.h"
#include "ship.h"
#include "sprite_component.h"

#include <algorithm>
#include <SDL3_image/SDL_image.h>

const int windowWidth = 1024;
const int windowHeight = 768;

Game::Game() :
    isRunning(true),
    window(nullptr),
    renderer(nullptr),
    ticksCount(0),
    updatingActors(true),
    ship(nullptr) {}

bool Game::initialize() {
    // Initialize SDL
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Creating Window
    window = SDL_CreateWindow("Game Programming - Pong", windowWidth, windowHeight, 0);
    if(!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Creating Renderer
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);
    SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);

    renderer = SDL_CreateRendererWithProperties(props);
    if(!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
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

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

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

void Game::processInput() {
    // Process SDL events
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;

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

void Game::generateOutput() {
    // Clearing renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    // Drawing objects
    for(auto sprite: sprites) {
        sprite->draw(renderer);
    }

    // Swapping buffers
    SDL_RenderPresent(renderer);
}

SDL_Texture* Game::getTexture(const std::string& filename) {
    SDL_Texture* tex = nullptr;

    // Is the texture already in the map?
    auto iter = textures.find(filename);
    if(iter != textures.end()) {
        tex = iter->second;
    } else {
        // Load from file
        SDL_Surface* surf = IMG_Load(filename.c_str());
        if(!surf) {
            SDL_Log("Failed to create surface for %s: %s", filename.c_str(), SDL_GetError());
            return nullptr;
        }

        tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_DestroySurface(surf);
        if(!tex) {
            SDL_Log("Failed to convert surface to texture for %s", filename.c_str());
            return nullptr;
        }

        textures.emplace(filename.c_str(), tex);
    }

    return tex;
}

void Game::addSprite(SpriteComponent* sprite) {
    // Find the insertion point in the sorted vector
    // (The first element with a higher draw order than me)
    int spriteDrawOrder = sprite->getDrawOrder();
    auto iter = sprites.begin();
    for(; iter != sprites.end(); iter++) {
        if(spriteDrawOrder < (*iter)->getDrawOrder()) {
            break;
        }
    }

    // Insert element before position of iterator
    sprites.insert(iter, sprite);
}

void Game::removeSprite(SpriteComponent* sprite) {
    auto iter = std::find(sprites.begin(), sprites.end(), sprite);
    sprites.erase(iter);
}

void Game::loadData() {
    // Create player's ship
    ship = new Ship(this);
    ship->setPosition(Vector2{ 100.0f, 384.0f });
    ship->setScale(1.0f);

    // Create actor for the background (this doesn't need a subclass)
    Actor* temp = new Actor(this);
    temp->setPosition(Vector2{ windowWidth / 2.0f, windowHeight / 2.0f });

    // Create new "far back" background
    BGSpriteComponent* bg = new BGSpriteComponent(temp);
    bg->setScreenSize(getScreenSize());
    std::vector<SDL_Texture*> bgtexs = { getTexture("assets/Farback01.png"),
        getTexture("assets/Farback02.png") };
    bg->setBGTextures(bgtexs);
    bg->setScrollSpeed(-100.0f);
    // Create the closer background
    bg = new BGSpriteComponent(temp);
    bg->setScreenSize(getScreenSize());
    bgtexs = { getTexture("assets/Stars.png"), getTexture("assets/Stars.png") };
    bg->setBGTextures(bgtexs);
    bg->setScrollSpeed(-200.0f);

    // Add Asteroids
    const int numAsteroids = 20;
    for(int i = 0; i < numAsteroids; i++) {
        new Asteroid(this);
    }
}

void Game::unloadData() {
    delete ship;
    // Delete actors
    while(!actors.empty()) {
        delete actors.back();
    }
    // Clear asteroids vector, as pointers are cleared by actor
    asteroids.clear();

    // Destroy textures
    for(auto i: textures) {
        SDL_DestroyTexture(i.second);
    }
    textures.clear();
}

Vector2 Game::getScreenSize() const {
    return Vector2{ windowWidth, windowHeight };
}

std::vector<Asteroid*>& Game::getAsteroids() {
    return asteroids;
}

void Game::addAsteroid(Asteroid* ast) {
    asteroids.emplace_back(ast);
}

void Game::removeAsteroid(Asteroid* ast) {
    auto iter = std::find(asteroids.begin(), asteroids.end(), ast);
    if(iter != asteroids.end()) {
        asteroids.erase(iter);
    }
}
