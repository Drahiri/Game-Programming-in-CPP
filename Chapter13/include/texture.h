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

    void setActive();

    int getWidth() const;
    int getHeight() const;

    void createFromSurface(SDL_Surface* surface);

private:
    // OpenGL of this texture
    unsigned int textureID;

    // Width/height of the texture
    int width;
    int height;
};

#endif