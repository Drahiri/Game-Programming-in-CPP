#version 330

// Inputs from vertex shader
in vec2 fragTexCoord; // Tex coord
in vec3 fragNormal;   // Normal (in world space)
in vec3 fragWorldPos; // Position (in world space)

// This corresponds to the outputs of G-buffer
layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

// Diffuse texture sampler
uniform sampler2D uTexture;

void main() {
    // Diffuse color is from texture
    outDiffuse = texture(uTexture, fragTexCoord).xyz;
    // Pass normal/world position directly along
    outNormal = fragNormal;
    outWorldPos = fragWorldPos;
}