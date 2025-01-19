#pragma once

#include <glm/glm.hpp>

class GameObject2D
{
public:
    GameObject2D() = default;

    GameObject2D(const glm::vec3& position);

    bool Intersects(const GameObject2D& other) const;

    glm::vec2 ResolveCollision(const GameObject2D& other) const;

    void SetPosition(float x, float y) { m_Position = { x, y, m_Position.z }; }
    void SetPosition(const glm::vec3& position) { m_Position = position; }
    void SetWidth(float width) { m_Width = width; }
    void SetHeight(float height) { m_Height = height; }

    const glm::vec3& GetPosition() const { return m_Position; }
    const float GetWidth() const { return m_Width; }
    const float GetHeight() const { return m_Height; }
protected:
    glm::vec3 m_Position;
    float m_Width, m_Height;
};