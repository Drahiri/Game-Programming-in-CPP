#version 330

// Inputs from vertex shader
in vec2 fragTexCoord; // Tex coord

layout(location = 0) out vec4 outColor;

// Different textures from G-buffer
uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;
uniform sampler2D uSpecPower;

// Lighting uniforms
// Struct for directional light
struct DirectionalLight {
    // Direction of light
    vec3 direction;
    // Diffuse color
    vec3 diffuseColor;
    // Specular color
    vec3 specColor;
};

// Camera position (in world space)
uniform vec3 uCameraPos;
// Ambient light level
uniform vec3 uAmbientLight;
// Directional Light (only one for now)
uniform DirectionalLight uDirLight;

void main() {
    // Sample diffuse color, normal, world position fragment
    vec3 gbufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
    vec3 gbufferNormal = texture(uGNormal, fragTexCoord).xyz;
    vec3 gbufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;
    float gbufferSpecPow = texture(uSpecPower, fragTexCoord).x;

    // Calculate Phong lighting
    // Surface normal
    vec3 N = normalize(gbufferNormal);
    // Vector from surface to light
    vec3 L = normalize(-uDirLight.direction);
    // Vector from surface to camera
    vec3 V = normalize(uCameraPos - gbufferWorldPos);
    // Reclection of -L about N
    vec3 R = normalize(reflect(-L, N));

    // Compute phong reclection
    vec3 Phong = uAmbientLight;
    float NdotL = dot(N, L);
    if(NdotL > 0) {
        vec3 Diffuse = uDirLight.diffuseColor * NdotL;
        vec3 Specular = uDirLight.specColor * pow(max(0.0, dot(R, V)), gbufferSpecPow);
        Phong += Diffuse + Specular;
    }

    // Clamp light between 0-1 RGB value
    Phong = clamp(Phong, 0.0f, 1.0f);

    // Final color is diffuse color times phong lighting
    outColor = vec4(gbufferDiffuse * Phong, 1.0);
}