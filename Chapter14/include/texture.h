#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL3/SDL_surface.h>
#include <string>

class Texture {
public:
    Texture();
    ~Texture();

    bool load(const std::string& fileName);
    void unload();

    void setActive(int index = 0);

    int getWidth() const;
    int getHeight() const;
    unsigned int getTextureID() const;
    const std::string& getFileName() const;

    void createFromSurface(SDL_Surface* surface);
    void createForRendering(int width, int height, unsigned int format);

private:
    // OpenGL of this texture
    unsigned int textureID;

    // Width/height of the texture
    int width;
    int height;

    std::string fileName;
};

#endif