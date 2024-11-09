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

layout (location = 0) in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D texture1;

void main()
{
    color = mix(texture(texture1, v_TexCoord), u_Color, 0.8);
}