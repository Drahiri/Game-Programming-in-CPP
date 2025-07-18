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
// Normal (in world space)
in vec3 fragNormal;
// Position (in world space)
in vec3 fragWorldPos;

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
    // Surface normal
    vec3 N = normalize(fragNormal);
    // Vector from surface to light
    vec3 L = normalize(-uDirLight.direction);
    // Vector from surface to camera
    vec3 V = normalize(uCameraPos - fragWorldPos);
    // Reclection of -L about N
    vec3 R = normalize(reflect(-L, N));

    // Compute phong reclection
    vec3 Phong = uAmbientLight;
    float NdotL = dot(N, L);
    if(NdotL > 0) {
        vec3 Diffuse = uDirLight.diffuseColor * NdotL;
        vec3 Specular = uDirLight.specColor * pow(max(0.0, dot(R, V)), uSpecPower);
        Phong += Diffuse + Specular;
    }

    // Sample color from texture
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}