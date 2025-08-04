// Request GLSL 3.3
#version 330

// Attribute 0 is position, 1 is normal, 2 is tex coords
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// Add texture coordinate as output
out vec2 fragTexCoord;

// Normal (in world space)
out vec3 fragNormal;
// Position (in world space)
out vec3 fragWorldPos;

// World and view-projection matrices
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

void main() {
    // Convert position to homogeneous coordinates
    vec4 pos = vec4(inPosition, 1.0);
    // Transform position to world space
    pos *= uWorldTransform;
    // Save world position
    fragWorldPos = pos.xyz;
    // Transform position to clip space
    gl_Position = pos * uViewProj;

    // Transform normal into world space (w = 0)
    fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

    // Pass along the texture coordinate to frag shader
    fragTexCoord = inTexCoord;
}