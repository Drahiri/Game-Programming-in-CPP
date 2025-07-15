#include "texture.h"

#include <GL/glew.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SOIL/SOIL.h>

Texture::Texture() : textureID(0), width(0), height(0) {}

Texture::~Texture() {
    unload();
}

bool Texture::load(const std::string& fileName) {
    // Loading texture image
    int channels = 0;
    unsigned char* image = SOIL_load_image(fileName.c_str(), // Name of file
          &width,                                            // Stores width
          &height,                                           // Stores height
          &channels,                                         // Stores number of channels
          SOIL_LOAD_AUTO                                     // Type of image file, or auto for any
    );

    if(image == nullptr) {
        SDL_Log("Failed to load texture %s: %s", fileName.c_str(), SOIL_last_result());
        return false;
    }

    int format = GL_RGB;
    if(channels == 4) {
        format = GL_RGBA;
    }

    // Generating texture object
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, // Texture target
          0,                    // Level of details (for now, assume 0)
          format,               // Color format OpenGL should use
          width,                // Width of texture
          height,               // Height of texture
          0,                    // Border - "this value must be 0"
          format,               // Color format of input data
          GL_UNSIGNED_BYTE,     // Bit depth of input data
                                // Unsigned byte specifies 8-bit channels
          image                 // Poiter to image data
    );

    // Free image data
    SOIL_free_image_data(image);

    // Enable bilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::unload() {
    glDeleteTextures(1, &textureID);
}

void Texture::setActive() {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

int Texture::getWidth() const {
    return width;
}

int Texture::getHeight() const {
    return height;
}

void Texture::createFromSurface(SDL_Surface* surface) {
    width = surface->w;
    height = surface->h;

    // Generate a GL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(
          GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    // Use linear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}