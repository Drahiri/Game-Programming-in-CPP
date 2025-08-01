#version 330

// Inputs from vertex shader
in vec2 fragTexCoord; // Tex coord

layout(location = 0) out vec4 outColor;

// Different textures from G-buffer
uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;
uniform sampler2D uSpecPower;

// Point Light uniforms
struct PointLight {
    // Position of light
    vec3 worldPos;
    // Diffuse color
    vec3 diffuseColor;
    // Radius of the light
    float innerRadius;
    float outerRadius;
    vec3 specColor;
};

uniform PointLight uPointLight;
uniform vec3 uCameraPos;
// Stores width/height of screen
uniform vec2 uScreenDimensions;

void main() {
    // Calculate the coordinate to sample into the G-buffer
    vec2 gBufferCoord = gl_FragCoord.xy / uScreenDimensions;

    // Sample from G-buffer
    vec3 gbufferDiffuse = texture(uGDiffuse, gBufferCoord).xyz;
    vec3 gbufferNorm = texture(uGNormal, gBufferCoord).xyz;
    vec3 gbufferWorldPos = texture(uGWorldPos, gBufferCoord).xyz;
    float gbufferSpecPower = texture(uSpecPower, gBufferCoord).x;

    // Calcuate normal and vector from surface to light
    vec3 N = normalize(gbufferNorm);
    vec3 L = normalize(uPointLight.worldPos - gbufferWorldPos);
    vec3 V = normalize(uCameraPos - gbufferWorldPos);
    vec3 R = normalize(reflect(-L, N));

    // Compute Phong diffuse component for the light
    vec3 Phong = vec3(0.0, 0.0, 0.0);
    float NdotL = dot(N, L);
    if(NdotL > 0) {
        // Get the distance between the light and the world pos
        float dist = distance(uPointLight.worldPos, gbufferWorldPos);
        // Use smoothstep to compute value in range [0,1]
        // between inner/outer radius
        float intensity = smoothstep(uPointLight.innerRadius, uPointLight.outerRadius, dist);
        // The diffuse color of the light depends on intensity
        vec3 diffuseColor = mix(uPointLight.diffuseColor, vec3(0.0, 0.0, 0.0), intensity);
        diffuseColor = diffuseColor * NdotL;
        vec3 Specular = uPointLight.specColor * pow(max(0.0, dot(R, V)), gbufferSpecPower);
        Specular = mix(Specular, vec3(0.0, 0.0, 0.0), intensity);
        Specular *= NdotL;

        Phong = diffuseColor + Specular;
    }

    // Final color is texture color times phong light
    outColor = vec4(gbufferDiffuse * Phong, 1.0);
}