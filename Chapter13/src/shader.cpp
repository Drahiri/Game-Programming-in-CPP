#include "shader.h"

#include <fstream>
#include <SDL3/SDL_log.h>
#include <sstream>

Shader::Shader() : vertexShader(0), fragShader(0), shaderProgram(0) {}

Shader::~Shader() {}

bool Shader::load(const std::string& vertName, const std::string& fragName) {
    // Compile vertex and fragment shaders
    if(!compileShader(vertName, GL_VERTEX_SHADER, vertexShader)
          || !compileShader(fragName, GL_FRAGMENT_SHADER, fragShader)) {
        return false;
    }

    // Now create a shader program that links together vertex/frag shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    // Verify that the program linked successfully
    if(!isValidProgram()) {
        return false;
    }
    return true;
}

void Shader::unload() {
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

void Shader::setActive() {
    glUseProgram(shaderProgram);
}

void Shader::setMatrixUniform(const char* name, const Matrix4& matrix) {
    // Find the uniform by this name
    GLuint loc = glGetUniformLocation(shaderProgram, name);

    // Send the matrix data to the uniform
    glUniformMatrix4fv(loc,      // Uniform ID
          1,                     // Number of matrices (only 1 in this case)
          GL_TRUE,               // Set to TRUE if using row vectors
          matrix.GetAsFloatPtr() // Pointer to matrix data
    );
}

void Shader::setMatrixUniforms(const char* name, Matrix4* matrices, unsigned int count) {
    GLuint loc = glGetUniformLocation(shaderProgram, name);
    glUniformMatrix4fv(loc, count, GL_TRUE, matrices->GetAsFloatPtr());
}

void Shader::setVec3Uniform(const char* name, const Vector3& vec) {
    GLuint loc = glGetUniformLocation(shaderProgram, name);
    glUniform3fv(loc, 1, vec.GetAsFloatPtr());
}

void Shader::setFloatUniform(const char* name, const float value) {
    GLuint loc = glGetUniformLocation(shaderProgram, name);
    glUniform1f(loc, value);
}

void Shader::setIntUniform(const char* name, const int value) {
    GLuint loc = glGetUniformLocation(shaderProgram, name);
    glUniform1i(loc, value);
}

bool Shader::compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader) {
    // Open file
    std::ifstream shaderFile(fileName);
    if(shaderFile.is_open()) {
        // Read all the text into a string
        std::stringstream sstream;
        sstream << shaderFile.rdbuf();

        std::string contents = sstream.str();
        const char* contentsChar = contents.c_str();

        // Create a shader of the specified type
        outShader = glCreateShader(shaderType);

        // Set the source characters and try to compile
        glShaderSource(outShader, 1, &(contentsChar), nullptr);
        glCompileShader(outShader);

        if(!isCompiled(outShader)) {
            SDL_Log("Failed to compile shader %s", fileName.c_str());
            return false;
        }
    } else {
        SDL_Log("Shader file not found: %s", fileName.c_str());
        return false;
    }

    return true;
}

bool Shader::isCompiled(GLuint shader) {
    GLint status;

    // Query the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE) {
        char buffer[512];
        memset(buffer, 0, 512);
        glGetShaderInfoLog(shader, 511, nullptr, buffer);
        SDL_Log("GLSL compile failed:\n%s", buffer);
        return false;
    }
    return true;
}

bool Shader::isValidProgram() {
    GLint status;

    // Queery the link status
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if(status != GL_TRUE) {
        char buffer[512];
        memset(buffer, 0, 512);
        glGetProgramInfoLog(shaderProgram, 511, nullptr, buffer);
        SDL_Log("GLSL link failed:\n%s", buffer);
        return false;
    }
    return true;
}
