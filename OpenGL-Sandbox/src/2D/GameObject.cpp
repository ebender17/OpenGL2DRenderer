#include "GameObject2D.h"

GameObject2D::GameObject2D(const glm::vec3& position)
    : m_Position(position), m_Width(0), m_Height(0)
{
}

bool GameObject2D::Intersects(const GameObject2D& other) const
{
    auto otherPosition = other.GetPosition();
    // collision x-axis
    bool collisionX = m_Position.x + m_Width >= otherPosition.x &&
        otherPosition.x + other.GetWidth() >= m_Position.x;
    // collision y-axis
    bool collisionY = m_Position.y + m_Height >= otherPosition.y &&
        otherPosition.y + other.GetHeight() >= m_Position.y;
    return collisionX && collisionY;
}

glm::vec2 GameObject2D::ResolveCollision(const GameObject2D& other) const
{
    auto otherPosition = other.GetPosition();
    float overlapX = std::min(
        otherPosition.x + other.GetWidth() - m_Position.x,
        m_Position.x + m_Width - otherPosition.x
    );
    float overlapY = std::min(
        otherPosition.y + other.GetHeight() - m_Position.y,
        m_Position.y + m_Height - otherPosition.y
    );

    if (overlapX < overlapY)
    {
        if (otherPosition.x < m_Position.x)
            return { -overlapX, 0 }; // push left
        else
            return { 0, overlapY }; // push right
    }
    else
    {
        if (otherPosition.y < m_Position.y)
            return { 0, -overlapY }; // push up
        else
            return { 0, overlapY }; // push down
    }
}