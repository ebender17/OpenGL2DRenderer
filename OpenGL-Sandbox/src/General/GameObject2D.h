#pragma once

#include <glm/glm.hpp>
#include <GLCore.h>

class GameObject2D
{
public:
    GameObject2D(const glm::vec3& position, const glm::vec2& spriteSize, float width, float height)
        : m_Position(position), m_SpriteSize(spriteSize), m_Width(width), m_Height(height)
    {
    }

    void SetPosition(float x, float y) { m_Position = { x, y, m_Position.z }; }
    void SetPosition(const glm::vec3& position) { m_Position = position; }

    const glm::vec3& GetPosition() const { return m_Position; }
    const glm::vec2& GetSpriteSize() const { return m_SpriteSize; }
    const float GetWidth() const { return m_Width; }
    const float GetHeight() const { return m_Height; }
protected:
    glm::vec3 m_Position;
    glm::vec2 m_SpriteSize;
    float m_Width;
    float m_Height;
};