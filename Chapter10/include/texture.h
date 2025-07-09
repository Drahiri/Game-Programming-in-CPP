#ifndef TEXTURE_H
#define TEXTURE_H

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

private:
    // OpenGL of this texture
    unsigned int textureID;

    // Width/height of the texture
    int width;
    int height;
};

#endif