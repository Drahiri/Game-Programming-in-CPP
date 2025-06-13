#include "game.h"

#include "actor.h"
#include "shader.h"
#include "sprite_component.h"
#include "texture.h"
#include "vertex_array.h"

#include <algorithm>
#include <GL/glew.h>
#include <SDL3_image/SDL_image.h>

const int windowWidth = 1024;
const int windowHeight = 768;

Game::Game() :
    isRunning(true),
    window(nullptr),
    context(nullptr),
    ticksCount(0),
    updatingActors(true) {}

bool Game::initialize() {
    // Initialize SDL
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Use the core OpenGL profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Specify version 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Request a color buffer with 8-bits per RGBA channels
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    // Enable double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Force OpenGL to use hardware acceleration
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // Creating Window
    window =
          SDL_CreateWindow("Game Programming - Pong", windowWidth, windowHeight, SDL_WINDOW_OPENGL);
    if(!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Creating context
    context = SDL_GL_CreateContext(window);
    if(!context) {
        SDL_Log("Failed to create context: %s", SDL_GetError());
        return false;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK) {
        SDL_Log("Failed to initialize GLEW.");
        return false;
    }

    // On some platforms, GLEW will emit a benign error code, so clear it
    glGetError();

    // Make sure we can create/compile shaders
    if(!loadShaders()) {
        SDL_Log("Failed to load shaders.");
        return false;
    }

    loadData();
    initSpriteVerts();

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

    SDL_GL_DestroyContext(context);
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

void Game::generateOutput() {
    // Set the clear color to gray
    glClearColor(0.86f, 0.86f, 0.86f, 1.0f);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Set sprite shader and vertex array objects active
    spriteShader->setActive();
    spriteVerts->setActive();

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,    // srcFactor is srcAlpha
          GL_ONE_MINUS_SRC_ALPHA // dstFactor is 1 - srcAlpha
    );

    // Drawing objects
    for(auto sprite: sprites) {
        sprite->draw(spriteShader);
    }

    // Swap the buffers, which also display the scene
    SDL_GL_SwapWindow(window);
}

Texture* Game::getTexture(const std::string& filename) {
    Texture* tex = nullptr;

    // Is the texture already in the map?
    auto iter = textures.find(filename);
    if(iter != textures.end()) {
        tex = iter->second;
    } else {
        // Load from file
        tex = new Texture();
        tex->load(filename);

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

void Game::loadData() {}

void Game::unloadData() {
    // Delete actors
    while(!actors.empty()) {
        delete actors.back();
    }

    // Destroy textures
    for(auto i: textures) {
        delete i.second;
    }
    textures.clear();
}

void Game::initSpriteVerts() {
    /* clang-format off */
    float vertices[] = {
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Bottom-left
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    /* clang-format on */

    spriteVerts = new VertexArray(vertices, 4, indices, 6);
}

bool Game::loadShaders() {
    spriteShader = new Shader();

    if(!spriteShader->load("shaders/sprite.vert", "shaders/sprite.frag")) {
        return false;
    }

    spriteShader->setActive();
    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(windowWidth, windowHeight);
    spriteShader->setMatrixUniform("uViewProj", viewProj);

    return true;
}

Vector2 Game::getScreenSize() const {
    return Vector2{ windowWidth, windowHeight };
}
