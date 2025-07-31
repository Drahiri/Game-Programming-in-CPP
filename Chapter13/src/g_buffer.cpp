#include "g_buffer.h"

#include "texture.h"

#include <GL/glew.h>

GBuffer::GBuffer() : bufferID(0) {}

GBuffer::~GBuffer() {}

bool GBuffer::create(int width, int height) {
    // Crerate the framebuffer object and save in bufferID
    glGenFramebuffers(1, &bufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, bufferID);

    // Add a depth buffer to this target
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    // Create textures for each output in the G-buffer
    for(int i = 0; i < NUM_GBUFFER_TEXTURES; i++) {
        Texture* tex = new Texture();
        // We want three 32-bit float components for each texture
        tex->createForRendering(width, height, GL_RGB32F);
        textures.emplace_back(tex);
        // Attach this texture to a color output
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex->getTextureID(), 0);
    }

    // Create a vector of the color attachments
    std::vector<GLenum> attachments;
    for(int i = 0; i < NUM_GBUFFER_TEXTURES; i++) {
        attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
    }

    // Set the list of buffers to draw to
    glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data());

    // Make sure everything worked
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        destroy();
        return false;
    }

    return true;
}

void GBuffer::destroy() {
    // Destroy all textures
    for(auto tex: textures) {
        tex->unload();
        delete tex;
    }

    // Delete framebuffer
    glDeleteFramebuffers(1, &bufferID);
}

Texture* GBuffer::getTexture(Type type) {
    if(textures.size() > 0) {
        return textures[type];
    } else {
        return nullptr;
    }
}

unsigned int GBuffer::getBufferID() const {
    return bufferID;
}

void GBuffer::setTexturesActive() {
    for(int i = 0; i < NUM_GBUFFER_TEXTURES; i++) {
        textures[i]->setActive(i);
    }
}