#type vertex
#version 460

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

void main()
{
    v_TexCoords = a_TexCoords;
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
}

#type fragment
#version 460

layout (location = 0) out vec4 color;

layout (location = 0) in vec2 v_TexCoords;

uniform sampler2D u_SceneTexture;
// Tile map texture (stores tile IDs)
uniform isampler2D u_TileMap;
// Inverse projection-view matrix to convert screen UVs to world space
uniform mat4 u_InvProjectionView;
// Tile size in world units
uniform float u_TileSize;

// Reflection effect uniforms
uniform float u_WaveAmplitude;
uniform float u_WaveFrequency;
uniform float u_ReflectionAlpha;
uniform vec4 u_WaterTint;

uniform float u_Time;

void main() {
    // Convert screen UV to clip space [-1, 1]
    vec2 clipPos = vec2(v_TexCoords.x * 2.0 - 1.0, v_TexCoords.y * 2.0 - 1.0);
    vec4 clipSpace = vec4(clipPos, 0.0, 1.0);

    // Convert clip space to world space using the inverse projection-view matrix
    vec4 worldPosH = u_InvProjectionView * clipSpace;
    vec3 worldPos = worldPosH.xyz / worldPosH.w;

    // Convert world position to tile indices
    ivec2 tileCoord = ivec2(
        floor(worldPos.x / u_TileSize),
        floor(worldPos.y / u_TileSize)
    );
    // TODO : clamp?
    int tileID = texelFetch(u_TileMap, tileCoord, 0).r;

    // Sample original scene texture normally
    vec4 sceneColor = texture(u_SceneTexture, v_TexCoords);

    bool isWater = (tileID == 1);

    if (!isWater) {
        // Not water? Just return the scene color
        color = sceneColor;
        return;
    }

    // Apply reflection for water pixels
    vec2 reflectionUV = vec2(
        v_TexCoords.x + u_WaveAmplitude * sin((v_TexCoords.y + u_Time) * u_WaveFrequency),
        clamp(1.0 - v_TexCoords.y, 0.0, 1.0) // Ensure UV stays valid
    );

    // Sample the scene texture again using the reflected UV
    vec4 reflectionColor = texture(u_SceneTexture, reflectionUV);

    // Blend the reflection with a tint
    // TODO : could we just use u_ReflectionAlpha?
    reflectionColor.rgb = mix(reflectionColor.rgb, u_WaterTint.rgb, u_WaterTint.a);
    reflectionColor.a   = u_ReflectionAlpha;

    color = reflectionColor;
}
