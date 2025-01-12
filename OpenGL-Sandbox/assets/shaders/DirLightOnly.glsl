#type vertex
#version 460

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;

layout (location = 0) out vec2 v_TexCoords;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec3 v_FragPos;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
    v_TexCoords = a_TexCoord;
    v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

    gl_Position = u_ViewProjection * vec4(v_FragPos, 1.0);
}

#type fragment
#version 460

layout (location = 0) out vec4 color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 

struct DirLight { 
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (location = 0) in vec2 v_TexCoords;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec3 v_FragPos;

uniform Material u_Material;

// Light uniforms
uniform vec3 u_ViewPos;
uniform float u_Time;
uniform DirLight u_DirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    vec3 result = CalcDirLight(u_DirLight, norm, viewDir);
    color = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specularMap = vec3(texture(u_Material.specular, v_TexCoords));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    vec3 emission = vec3(0.0);
    if (specularMap.r == 0.0) // check for mask
    {
        emission = texture(u_Material.emission, v_TexCoords).rgb;
        emission = emission * (sin(u_Time) * 0.5 + 0.5) * 2.0;  // fading
    }

    vec3 ambient  = light.ambient  * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 specular = light.specular * spec * specularMap;
    return (ambient + diffuse + specular + emission);
}