// Basic Texture Shader

#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

layout(std140, binding = 0) uniform CameraData {
    mat4 u_ViewProjection;
};

out VS_OUT
{
    vec4 Color;
    vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
} vs_out;

#ifdef WATER_REFLECTION
    // TODO : add needed uniforms
#endif

void main()
{
    vec3 position = a_Position;

#ifdef WATER_REFLECTION
    // TODO : expand a_Position so sprite is not clipped
#endif

    vs_out.Color = a_Color;
    vs_out.TexCoord = a_TexCoord;
    vs_out.TexIndex = a_TexIndex;
    vs_out.TilingFactor = a_TilingFactor;


    gl_Position = u_ViewProjection * vec4(position, 1.0);
}

#type fragment
#version 460 core

#ifdef WATER_REFLECTION
    #include "WaterReflections.glsl"
#endif

layout(location = 0) out vec4 color;

in VS_OUT
{
    vec4 Color;
    vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
} fs_in;

layout (binding = 0) uniform sampler2D u_Textures[32];

#ifdef WATER_REFLECTION
uniform float u_Time;

// TODO : add needed uniforms

uniform vec3 u_WaterReflectionColor;
uniform float u_WaterReflectionColorMix;
uniform float u_WaterReflectionAlpha;
#endif

void main()
{
    vec4 texColor = fs_in.Color;

    vec2 uv = fs_in.TexCoord;
    #ifdef WATER_REFLECTION
        // TODO : adjust UVs

        texColor.rgb = mix(texColor.rgb, u_WaterReflectionColor, u_WaterReflectionColorMix);
        texColor.a *= u_WaterReflectionAlpha;
    #endif

    switch(int(fs_in.TexIndex))
    {
        case  0: texColor *= texture(u_Textures[ 0], uv * fs_in.TilingFactor); break;
        case  1: texColor *= texture(u_Textures[ 1], uv * fs_in.TilingFactor); break;
        case  2: texColor *= texture(u_Textures[ 2], uv * fs_in.TilingFactor); break;
        case  3: texColor *= texture(u_Textures[ 3], uv * fs_in.TilingFactor); break;
        case  4: texColor *= texture(u_Textures[ 4], uv * fs_in.TilingFactor); break;
        case  5: texColor *= texture(u_Textures[ 5], uv * fs_in.TilingFactor); break;
        case  6: texColor *= texture(u_Textures[ 6], uv * fs_in.TilingFactor); break;
        case  7: texColor *= texture(u_Textures[ 7], uv * fs_in.TilingFactor); break;
        case  8: texColor *= texture(u_Textures[ 8], uv * fs_in.TilingFactor); break;
        case  9: texColor *= texture(u_Textures[ 9], uv * fs_in.TilingFactor); break;
        case 10: texColor *= texture(u_Textures[10], uv * fs_in.TilingFactor); break;
        case 11: texColor *= texture(u_Textures[11], uv * fs_in.TilingFactor); break;
        case 12: texColor *= texture(u_Textures[12], uv * fs_in.TilingFactor); break;
        case 13: texColor *= texture(u_Textures[13], uv * fs_in.TilingFactor); break;
        case 14: texColor *= texture(u_Textures[14], uv * fs_in.TilingFactor); break;
        case 15: texColor *= texture(u_Textures[15], uv * fs_in.TilingFactor); break;
        case 16: texColor *= texture(u_Textures[16], uv * fs_in.TilingFactor); break;
        case 17: texColor *= texture(u_Textures[17], uv * fs_in.TilingFactor); break;
        case 18: texColor *= texture(u_Textures[18], uv * fs_in.TilingFactor); break;
        case 19: texColor *= texture(u_Textures[19], uv * fs_in.TilingFactor); break;
        case 20: texColor *= texture(u_Textures[20], uv * fs_in.TilingFactor); break;
        case 21: texColor *= texture(u_Textures[21], uv * fs_in.TilingFactor); break;
        case 22: texColor *= texture(u_Textures[22], uv * fs_in.TilingFactor); break;
        case 23: texColor *= texture(u_Textures[23], uv * fs_in.TilingFactor); break;
        case 24: texColor *= texture(u_Textures[24], uv * fs_in.TilingFactor); break;
        case 25: texColor *= texture(u_Textures[25], uv * fs_in.TilingFactor); break;
        case 26: texColor *= texture(u_Textures[26], uv * fs_in.TilingFactor); break;
        case 27: texColor *= texture(u_Textures[27], uv * fs_in.TilingFactor); break;
        case 28: texColor *= texture(u_Textures[28], uv * fs_in.TilingFactor); break;
        case 29: texColor *= texture(u_Textures[29], uv * fs_in.TilingFactor); break;
        case 30: texColor *= texture(u_Textures[30], uv * fs_in.TilingFactor); break;
        case 31: texColor *= texture(u_Textures[31], uv * fs_in.TilingFactor); break;
    }

    if(texColor.a < 0.0001)
        discard;
    color = texColor;
}