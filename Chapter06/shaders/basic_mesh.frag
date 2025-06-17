// Request GLSL 3.3
#version 330

// Tex coord input from vertex shader
in vec2 fragTexCoord;

// For texture sampling
uniform sampler2D uTexture;

// Output color
out vec4 outColor;

void main() {
    // Sample color from texture
    outColor = texture(uTexture, fragTexCoord);
}