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

void main() {
    color = texture(u_SceneTexture, v_TexCoords);
}
