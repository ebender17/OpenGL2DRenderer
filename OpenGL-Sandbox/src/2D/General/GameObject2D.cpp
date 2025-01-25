#include "GameObject2D.h"

GameObject2D::GameObject2D(const glm::vec3& position, const glm::vec2& spriteSize, float width, float height)
    : m_Position(position), m_SpriteSize(spriteSize), m_Width(width), m_Height(height)
{
}

bool GameObject2D::IntersectsTile(const glm::vec2& position) const
{
    // collision x-axis
    bool collisionX = m_Position.x >= position.x; 
    // collision y-axis
    bool collisionY = m_Position.y >= position.y + 1.0f; // (other.height / other.height) = 1.0f
    return collisionX && collisionY;
}