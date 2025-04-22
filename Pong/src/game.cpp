#include "game.h"

const int windowWidth = 1024;
const int windowHeight = 768;
const int thickness = 15;
const float paddleH = 100.0f;
const float paddleSpeed = 300.0f;

Game::Game() :
    isRunning(true),
    window(nullptr),
    renderer(nullptr),
    ticksCount(0),
    ballPos({ windowWidth / 2.0f, windowHeight / 2.0f }),
    ballVel({ -200.0f, 235.0f }),
    paddlePos({ 10.0f, windowHeight / 2.0f }),
    paddleDir(0) {}

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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
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

    // Paddle position
    paddleDir = 0;
    if(state[SDL_SCANCODE_W]) {
        paddleDir -= 1;
    }
    if(state[SDL_SCANCODE_S]) {
        paddleDir += 1;
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

    // Moving paddle
    if(paddleDir != 0) {
        paddlePos.y += paddleDir * paddleSpeed * deltaTime;

        // Make sure paddle doesn't move off screen
        if(paddlePos.y < (paddleH / 2.0f + thickness)) {
            paddlePos.y = paddleH / 2.0f + thickness;
        }

        else if(paddlePos.y > (windowHeight - paddleH / 2.0f - thickness)) {
            paddlePos.y = windowHeight - paddleH / 2.0f - thickness;
        }
    }

    // Moving ball
    ballPos.x += ballVel.x * deltaTime;
    ballPos.y += ballVel.y * deltaTime;

    // Intersect with paddle
    float diff = paddlePos.y - ballPos.y;
    // Absolute of diff
    diff = (diff > 0.0f) ? diff : -diff;

    // Collision
    // Paddle or right wall
    if(diff <= paddleH / 2.0f && ballPos.x <= (paddlePos.x + thickness / 2.0f)
          && ballPos.x > (paddlePos.x - thickness / 2.0f) && ballVel.x < 0.0f) {
        ballVel.x *= -1.0f;
    } else if(ballPos.x >= windowWidth - thickness && ballVel.x > 0.0f) {
        ballVel.x *= -1;
    }
    // End game if off screen
    else if(ballPos.x < 0.0f) {
        isRunning = false;
    }

    // Top/bottom walls
    if(ballPos.y <= thickness && ballVel.y < 0.0f) {
        ballVel.y *= -1;
    } else if(ballPos.y >= windowHeight - thickness && ballVel.y > 0.0f) {
        ballVel.y *= -1;
    }
}

void Game::generateOutput() {
    // Clearing renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    // Drawing objects
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Top wall
    SDL_FRect wall{ 0, 0, windowWidth, thickness };
    SDL_RenderFillRect(renderer, &wall);

    // Bottom wall
    wall.y = windowHeight - thickness;
    SDL_RenderFillRect(renderer, &wall);

    // Right wall
    wall.x = windowWidth - thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = windowHeight;
    SDL_RenderFillRect(renderer, &wall);

    // Ball
    SDL_FRect ball{ static_cast<int>(ballPos.x - thickness / 2.0),
        static_cast<int>(ballPos.y - thickness / 2.0f),
        thickness,
        thickness };
    SDL_RenderFillRect(renderer, &ball);

    // Paddle
    SDL_FRect paddle{ static_cast<int>(paddlePos.x),
        static_cast<int>(paddlePos.y - paddleH / 2.0f),
        thickness,
        paddleH };
    SDL_RenderFillRect(renderer, &paddle);

    // Swapping buffers
    SDL_RenderPresent(renderer);
}
