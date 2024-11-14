#pragma once

#include <glm/glm.hpp>

struct DirectionalLight
{
    glm::vec3 Direction;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;

    DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient,
        const glm::vec3& diffuse, const glm::vec3& specular) :
        Direction(direction), Ambient(ambient), Diffuse(diffuse), Specular(specular)
    {
    }
};