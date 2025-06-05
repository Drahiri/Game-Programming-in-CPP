#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

class Shader {
public:
    Shader();
    ~Shader();

    // Load the vertex/fragment shaders with the given names
    bool load(const std::string& vertName, const std::string& fragName);
    void unload();

    // Set this as the active shader program
    void setActive();

private:
    // Tries to compile the specified shader
    bool compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
    // Tests whether shader compiled successfully
    bool isCompiled(GLuint shader);
    // Tests whether vertex/fragment shader programs link
    bool isValidProgram();
    // Store the shader object IDs
    GLuint vertexShader;
    GLuint fragShader;
    GLuint shaderProgram;
};

#endif
