// Request GLSL 3.3
#version 330

// Vertex attributes
in vec3 inPosition;

// World and view-projection matrices
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

void main() {
    vec4 pos = vec4(inPosition, 1.0);
    gl_Position = pos * uWorldTransform * uViewProj;
}