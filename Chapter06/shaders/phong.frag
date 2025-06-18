// Request GLSL 3.3
#version 330

// Create a struct for directional light
struct DirectionalLight {
    // Direction of light
    vec3 direction;
    // Diffuse color
    vec3 diffuseColor;
    // Specular color
    vec3 specColor;
};

// Tex coord input from vertex shader
in vec2 fragTexCoord;

// For texture sampling
uniform sampler2D uTexture;

// Uniforms for lighting
// Camera position (in world space)
uniform vec3 uCameraPos;
// Ambient light level
uniform vec3 uAmbientLight;
// Specular power for this surface
uniform float uSpecPower;
// Directional Light (only one for now)
uniform DirectionalLight uDirLight;

// Output color
out vec4 outColor;

void main() {
    // Sample color from texture
    outColor = texture(uTexture, fragTexCoord);
}