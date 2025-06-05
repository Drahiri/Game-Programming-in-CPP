// Request GLSL 3.3
#version 330

// Any vertex attributes go here
in vec3 inPosition;

void main() {
    gl_Position = vec4(inPosition, 1.0);
}