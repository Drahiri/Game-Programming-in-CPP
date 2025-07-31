#ifndef SHADER_H
#define SHADER_H

#include "math.h"

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

    // Set uniforms
    void setMatrixUniform(const char* name, const Matrix4& matrix);
    void setMatrixUniforms(const char* name, Matrix4* matrices, unsigned int count);
    void setVec3Uniform(const char* name, const Vector3& vec);
    void setFloatUniform(const char* name, const float value);
    void setIntUniform(const char* name, const int value);

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
