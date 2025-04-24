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
    paddlePos1({ 10.0f + thickness / 2.0f, windowHeight / 2.0f }),
    paddleDir1(0),
    paddlePos2({ windowWidth - thickness / 2.0f - 10.0f, windowHeight / 2.0f }),
    paddleDir2(0) {}

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

    // Update balls
    Ball ball{};
    ball.position = { windowWidth / 2.0f, windowHeight / 2.0f };
    ball.velocity = { -200.0f, 235.0f };
    balls.push_back(ball);

    ball.velocity = { 200.0f, -235.0f };
    balls.push_back(ball);

    ball.velocity = { 150.0f, 285.0f };
    balls.push_back(ball);

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
    paddleDir1 = 0;
    if(state[SDL_SCANCODE_W]) {
        paddleDir1 -= 1;
    }
    if(state[SDL_SCANCODE_S]) {
        paddleDir1 += 1;
    }

    paddleDir2 = 0;
    if(state[SDL_SCANCODE_I]) {
        paddleDir2 -= 1;
    }
    if(state[SDL_SCANCODE_K]) {
        paddleDir2 += 1;
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
    if(paddleDir1 != 0) {
        paddlePos1.y += paddleDir1 * paddleSpeed * deltaTime;

        // Make sure paddle doesn't move off screen
        if(paddlePos1.y < (paddleH / 2.0f + thickness)) {
            paddlePos1.y = paddleH / 2.0f + thickness;
        }

        else if(paddlePos1.y > (windowHeight - paddleH / 2.0f - thickness)) {
            paddlePos1.y = windowHeight - paddleH / 2.0f - thickness;
        }
    }

    if(paddleDir2 != 0) {
        paddlePos2.y += paddleDir2 * paddleSpeed * deltaTime;

        // Make sure paddle doesn't move off screen
        if(paddlePos2.y < (paddleH / 2.0f + thickness)) {
            paddlePos2.y = paddleH / 2.0f + thickness;
        }

        else if(paddlePos2.y > (windowHeight - paddleH / 2.0f - thickness)) {
            paddlePos2.y = windowHeight - paddleH / 2.0f - thickness;
        }
    }

    for(Ball& ball: balls) {
        // Moving ball
        ball.position.x += ball.velocity.x * deltaTime;
        ball.position.y += ball.velocity.y * deltaTime;

        // Intersect with paddle
        float diff1 = paddlePos1.y - ball.position.y;
        // Absolute of diff
        diff1 = (diff1 > 0.0f) ? diff1 : -diff1;

        float diff2 = paddlePos2.y - ball.position.y;
        diff2 = (diff2 > 0.0f) ? diff2 : -diff2;

        // Collision
        // Paddle1
        if(diff1 <= paddleH / 2.0f && ball.position.x <= (paddlePos1.x + thickness / 2.0f)
              && ball.position.x >= (paddlePos1.x - thickness / 2.0f) && ball.velocity.x < 0.0f) {
            ball.velocity.x *= -1.0f;
        }
        // Paddle 2
        else if(diff2 <= paddleH / 2.0f && ball.position.x >= (paddlePos2.x - thickness / 2.0f)
                && ball.position.x <= (paddlePos2.x + thickness / 2.0f) && ball.velocity.x > 0.0f) {
            ball.velocity.x *= -1;
        }
        // End game if off screen
        else if(ball.position.x < 0.0f || ball.position.x > windowWidth) {
            isRunning = false;
        }

        // Top/bottom walls
        if(ball.position.y <= thickness && ball.velocity.y < 0.0f) {
            ball.velocity.y *= -1;
        } else if(ball.position.y >= windowHeight - thickness && ball.velocity.y > 0.0f) {
            ball.velocity.y *= -1;
        }
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

    // Ball

    SDL_FRect ball{ 0, 0, thickness, thickness };

    for(Ball& ballObj: balls) {
        ball.x = static_cast<int>(ballObj.position.x - thickness / 2.0f);
        ball.y = static_cast<int>(ballObj.position.y - thickness / 2.0f);
        SDL_RenderFillRect(renderer, &ball);
    }

    // Paddle
    SDL_FRect paddle{ static_cast<int>(paddlePos1.x - thickness / 2.0f),
        static_cast<int>(paddlePos1.y - paddleH / 2.0f),
        thickness,
        paddleH };
    SDL_RenderFillRect(renderer, &paddle);

    paddle.x = static_cast<int>(paddlePos2.x - thickness / 2.0f);
    paddle.y = static_cast<int>(paddlePos2.y - paddleH / 2.0f);
    SDL_RenderFillRect(renderer, &paddle);

    // Swapping buffers
    SDL_RenderPresent(renderer);
}
