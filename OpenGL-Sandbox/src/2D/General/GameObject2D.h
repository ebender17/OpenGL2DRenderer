#pragma once

#include <glm/glm.hpp>
#include <GLCore.h>

class GameObject2D
{
public:
    GameObject2D(const glm::vec3& position, const glm::vec2& spriteSize, float width, float height);

    bool IntersectsTile(const glm::vec2& position) const;

    void SetPosition(float x, float y) { m_Position = { x, y, m_Position.z }; }
    void SetPosition(const glm::vec3& position) { m_Position = position; }
    void SetIsColliding(bool isColliding) { m_IsColliding = isColliding; }

    const glm::vec3& GetPosition() const { return m_Position; }
    const glm::vec2& GetSpriteSize() const { return m_SpriteSize; }
    float GetWidth() const { return m_Width; }
    float GetHeight() const { return m_Height; }
    bool GetIsColliding() const { return m_IsColliding; }
protected:
    glm::vec3 m_Position;
    glm::vec2 m_SpriteSize;
    float m_Width;
    float m_Height;

    bool m_IsColliding = false;
};