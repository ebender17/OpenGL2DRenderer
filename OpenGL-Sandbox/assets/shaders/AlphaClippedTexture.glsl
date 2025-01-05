#type vertex
#version 460

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec2 v_TexCoords;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
    v_TexCoords = a_TexCoord;
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 460

layout (location = 0) out vec4 color;

layout (location = 0) in vec2 v_TexCoords;

uniform sampler2D u_Texture;
uniform float u_AlphaThreshold;
uniform vec4 u_Color;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoords);
    if (texColor.a < u_AlphaThreshold)
        discard;
    color = u_Color * texColor;
}