#type vertex
#version 460

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;

layout (location = 0) out vec2 v_TexCoord;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec3 v_FragPos;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
    v_TexCoord = a_TexCoord;
    v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

    gl_Position = u_ViewProjection * vec4(v_FragPos, 1.0);
}

#type fragment
#version 460

layout (location = 0) out vec4 color;

layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec3 v_FragPos;

uniform vec4 u_Color;
uniform sampler2D texture1;

// Light uniforms
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor;

    // diffuse
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    float diffuseComponent = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseComponent * u_LightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-viewDir, norm);
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * specularComponent * u_LightColor;

    vec4 lighting = vec4((ambient + diffuse + specular), 1.0) * u_Color;
    color = texture(texture1, v_TexCoord) * lighting;
}