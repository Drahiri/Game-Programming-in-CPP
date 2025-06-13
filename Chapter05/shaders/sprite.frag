// Request GLSL 3.3
#version 330

// Tex coord input from vertex shader
in vec2 fragTexCoord;
in vec3 fragColor;

// For texture sampling
uniform sampler2D uTexture;

// Output color
out vec4 outColor;

void main() {
    // Sample color from texture
    vec4 textureColor = texture(uTexture, fragTexCoord);
    vec3 avarageColor = (textureColor.xyz + fragColor) / 2.0f;

    outColor = vec4(avarageColor, textureColor.w);
}