#ifndef G_BUFFER_H
#define G_BUFFER_H

#include <vector>

class Texture;

class GBuffer {
public:
    // Different types of data stored in the G-buffer
    enum Type {
        EDiffuse = 0,
        ENormal,
        EWorldPos,
        NUM_GBUFFER_TEXTURES
    };

    GBuffer();
    ~GBuffer();

    // Create/destroy the G-buffer
    bool create(int width, int height);
    void destroy();

    // Get the texture for a specific type of data
    Texture* getTexture(Type type);
    // Get the framebuffer objectID
    unsigned int getBufferID() const;
    // Setup all the G-buffer textures for sampling
    void setTexturesActive();

private:
    // Textures associated with G-buffer
    std::vector<Texture*> textures;
    // Framebuffer object ID
    unsigned int bufferID;
};

#endif