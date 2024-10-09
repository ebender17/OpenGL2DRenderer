#type vertex
#version 460

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position, 1.0);
}

#type fragment
#version 460

layout (location = 0) out vec4 color;

uniform vec4 u_Color;

layout (location = 0) in vec2 v_TexCoord;

void main()
{
    // color = u_Color;
    color = vec4(v_TexCoord, 0.0, 1.0);
}