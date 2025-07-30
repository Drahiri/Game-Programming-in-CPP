#include "renderer.h"

#include "game.h"
#include "mesh.h"
#include "mesh_component.h"
#include "shader.h"
#include "skeletal_mesh_component.h"
#include "sprite_component.h"
#include "texture.h"
#include "ui_screen.h"
#include "vertex_array.h"

#include <algorithm>
#include <GL/glew.h>

Renderer::Renderer(Game* game) :
    game(game),
    window(nullptr),
    viewMatrix(Matrix4::Identity),
    projectionMatrix(Matrix4::Identity),
    spriteShader(nullptr),
    spriteVerts(nullptr),
    meshShader(nullptr) {}

Renderer::~Renderer() {}

bool Renderer::initialize(float windowWidth, float windowHeight) {
    screenWidth = windowWidth;
    screenHeight = windowHeight;

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

    // Enable z-buffering
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

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

    setRelativeMouse(true);

    // Create quad for drawing sprites
    createSpriteVerts();

    // Create mirror framebuffer
    if(!createMirrorTarget()) {
        SDL_Log("Failed to create mirror framebuffer");
        return false;
    }

    return true;
}

void Renderer::shutdown() {
    delete spriteVerts;
    spriteShader->unload();
    delete spriteShader;

    meshShader->unload();
    delete meshShader;

    if(mirrorTexture != nullptr) {
        glDeleteFramebuffers(1, &mirrorBuffer);
        mirrorTexture->unload();
        delete mirrorTexture;
    }

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
}

void Renderer::unloadData() {
    // Destroy textures
    for(auto i: textures) {
        i.second->unload();
        delete i.second;
    }
    textures.clear();

    // Destroy meshes
    for(auto i: meshes) {
        i.second->unload();
        delete i.second;
    }
    meshes.clear();
}

void Renderer::draw() {
    // Draw to the mirror texture first
    draw3DScene(mirrorBuffer, mirrorView, projectionMatrix, 0.25f);
    // Draw to normal buffer
    draw3DScene(0, viewMatrix, projectionMatrix, 1.0f);

    // Draw all sprite components
    // Disable depth buffering
    glDisable(GL_DEPTH_TEST);
    // Enable alpha blending on the color buffer
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    // Set shader/vao as active
    spriteShader->setActive();
    spriteVerts->setActive();
    for(auto sprite: sprites) {
        if(sprite->getVisible()) {
            sprite->draw(spriteShader);
        }
    }

    for(auto ui: game->getUIStack()) {
        ui->draw(spriteShader);
    }

    // Swap the buffers, which also display the scene
    SDL_GL_SwapWindow(window);
}

void Renderer::draw3DScene(
      unsigned int framebuffer, const Matrix4& view, const Matrix4& proj, float viewportScale) {
    // Set the current framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Set viewport size based on scale
    glViewport(0,
          0,
          static_cast<int>(screenWidth) * viewportScale,
          static_cast<int>(screenHeight) * viewportScale);

    // Clear color buffer/depth buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw meshes
    // Enable depth buffering/disable alpha blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    meshShader->setActive();
    meshShader->setMatrixUniform("uViewProj", view * proj);
    setLightUniforms(meshShader);

    for(auto mc: meshComps) {
        if(mc->getVisible()) {
            mc->draw(meshShader);
        }
    }

    // Draw any skinned meshes now
    skinnedShader->setActive();
    skinnedShader->setMatrixUniform("uViewProj", view * proj);
    setLightUniforms(skinnedShader);

    for(auto sk: skeletalMeshes) {
        if(sk->getVisible()) {
            sk->draw(skinnedShader);
        }
    }
}

void Renderer::addSprite(SpriteComponent* sprite) {
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

void Renderer::removeSprite(SpriteComponent* sprite) {
    auto iter = std::find(sprites.begin(), sprites.end(), sprite);
    sprites.erase(iter);
}

void Renderer::addMeshComp(MeshComponent* meshComp) {
    if(meshComp->getIsSkeletal()) {
        SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(meshComp);
        skeletalMeshes.emplace_back(sk);
    } else {
        meshComps.emplace_back(meshComp);
    }
}

void Renderer::removeMeshComp(MeshComponent* meshComp) {
    if(meshComp->getIsSkeletal()) {
        SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(meshComp);
        auto iter = std::find(skeletalMeshes.begin(), skeletalMeshes.end(), sk);
        if(iter != skeletalMeshes.end()) {
            skeletalMeshes.erase(iter);
        }
    } else {
        auto iter = std::find(meshComps.begin(), meshComps.end(), meshComp);
        if(iter != meshComps.end()) {
            meshComps.erase(iter);
        }
    }
}

Texture* Renderer::getTexture(const std::string& filename) {
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

Texture* Renderer::getMirrorTexture() {
    return mirrorTexture;
}

Mesh* Renderer::getMesh(const std::string& fileName) {
    Mesh* m = nullptr;

    auto iter = meshes.find(fileName);
    if(iter != meshes.end()) {
        m = iter->second;
    } else {
        m = new Mesh();
        if(m->load(fileName, this)) {
            meshes.emplace(fileName, m);
        } else {
            delete m;
            m = nullptr;
        }
    }
    return m;
}

float Renderer::getScreenWidth() const {
    return screenWidth;
}

float Renderer::getScreenHeight() const {
    return screenHeight;
}

void Renderer::setViewMatrix(const Matrix4& view) {
    viewMatrix = view;
}

void Renderer::setMirrorView(const Matrix4& view) {
    mirrorView = view;
}

void Renderer::setAmbientLight(const Vector3& light) {
    ambientLight = light;
}

DirectionalLight& Renderer::getDirectionalLight() {
    return dirLight;
}

void Renderer::setLightUniforms(Shader* shader) {
    // Camera position is from inverted view
    Matrix4 invView = viewMatrix;
    invView.Invert();
    shader->setVec3Uniform("uCameraPos", invView.GetTranslation());
    // Ambient light
    shader->setVec3Uniform("uAmbientLight", ambientLight);
    // Directional Light
    shader->setVec3Uniform("uDirLight.direction", dirLight.direction);
    shader->setVec3Uniform("uDirLight.diffuseColor", dirLight.diffuseColor);
    shader->setVec3Uniform("uDirLight.specColor", dirLight.specColor);
}

Vector3 Renderer::unproject(const Vector3& screenPoint) const {
    // Convert screenPoint to device coordinates (between -1 and +1)
    Vector3 deviceCoord = screenPoint;
    deviceCoord.x /= (screenWidth) * 0.5f;
    deviceCoord.y /= (screenHeight) * 0.5f;

    // Transform vector by unprojection matrix
    Matrix4 unprojection = viewMatrix * projectionMatrix;
    unprojection.Invert();
    return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

void Renderer::getScreenDirection(Vector3& outStart, Vector3& outDir) const {
    // Get start point (in center of screen on near plane)
    Vector3 screenPoint(0.0f, 0.0f, 0.0f);
    outStart = unproject(screenPoint);

    // Get end of point (in center of screen, between near and far)
    screenPoint.z = 0.9f;
    Vector3 end = unproject(screenPoint);

    // Get direction vector
    outDir = end - outStart;
    outDir.Normalize();
}

void Renderer::setRelativeMouse(bool relative) {
    SDL_SetWindowRelativeMouseMode(window, relative);
    // Make an initial call to get relative to clear out
    if(relative) {
        SDL_GetRelativeMouseState(nullptr, nullptr);
    }
}

bool Renderer::createMirrorTarget() {
    int width = static_cast<int>(screenWidth) / 4;
    int height = static_cast<int>(screenHeight) / 4;

    // Generate a framebufer for the mirror texture
    glGenFramebuffers(1, &mirrorBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mirrorBuffer);

    // Crreate the texture we'll use for rendering
    mirrorTexture = new Texture();
    mirrorTexture->createForRendering(width, height, GL_RGB);

    // Add a depth buffer to this target
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    // Attach mirror texture as the output target for this framebuffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mirrorTexture->getTextureID(), 0);

    // Set the list of buffer to draw for this framebuffer
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    // Make sure everything worked
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        // If it didn't work, delete the framebuffer,
        // unload/delete the texture and return false
        glDeleteFramebuffers(1, &mirrorBuffer);
        mirrorTexture->unload();
        delete mirrorTexture;
        mirrorTexture = nullptr;
        return false;
    }

    return true;
}

bool Renderer::loadShaders() {
    // Load sprite shader
    spriteShader = new Shader();
    if(!spriteShader->load("shaders/sprite.vert", "shaders/sprite.frag")) {
        return false;
    }

    spriteShader->setActive();
    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(screenWidth, screenHeight);
    spriteShader->setMatrixUniform("uViewProj", viewProj);

    // Load mesh shader
    meshShader = new Shader();
    if(!meshShader->load("shaders/phong.vert", "shaders/phong.frag")) {
        return false;
    }
    meshShader->setActive();
    // Set the view-projection matrix
    viewMatrix = Matrix4::CreateLookAt(Vector3::Zero, // Camera position
          Vector3::UnitX,                             // Target position
          Vector3::UnitZ                              // Up
    );

    projectionMatrix = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), // Horizontal FOV
          screenWidth,                                                       // Width of view
          screenHeight,                                                      // Height of view
          25.0f,                                                             // Near plane distance
          10000.0f                                                           // Far plane distance
    );
    meshShader->setMatrixUniform("uViewProj", viewMatrix * projectionMatrix);

    // Load skeletal shader
    skinnedShader = new Shader();
    if(!skinnedShader->load("shaders/skinned.vert", "shaders/phong.frag")) {
        return false;
    }
    skinnedShader->setActive();
    skinnedShader->setMatrixUniform("uViewProj", viewMatrix * projectionMatrix);

    return true;
}

void Renderer::createSpriteVerts() {
    /* clang-format off */
    float vertices[] = {
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // Bottom-left
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    /* clang-format on */

    spriteVerts = new VertexArray(VertexArray::Layout::PosNormTex, vertices, 4, indices, 6);
}