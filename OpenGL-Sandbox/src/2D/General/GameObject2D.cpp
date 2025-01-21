#include "GameObject2D.h"

GameObject2D::GameObject2D(const glm::vec3& position, const glm::vec2& spriteSize, float width, float height)
    : m_Position(position), m_Rect(spriteSize, width, height)
{
}

void GameObject2D::OnEvent(GLCore::Event& event)
{
    PROFILE_FUNCTION();

    GLCore::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<GLCore::WindowResizeEvent>(GLCORE_BIND_EVENT_FN(GameObject2D::OnWindowResized));
}

bool GameObject2D::OnWindowResized(GLCore::WindowResizeEvent& event)
{
    float width = event.GetWidth();
    float height = event.GetHeight();
    m_Rect.Width = (m_Rect.WidthPixels / width) * 2;
    m_Rect.Height = (m_Rect.HeightPixels / height) * 2;
    return false;
}

bool GameObject2D::Intersects(const GameObject2D& other) const
{
    auto otherPosition = other.GetPosition();
    // collision x-axis
    bool collisionX = m_Position.x + m_Rect.Width >= otherPosition.x &&
        otherPosition.x + other.GetRect().Width >= m_Position.x;
    // collision y-axis
    bool collisionY = m_Position.y + m_Rect.Height >= otherPosition.y &&
        otherPosition.y + other.GetRect().Height >= m_Position.y;
    return collisionX && collisionY;
}

glm::vec2 GameObject2D::ResolveCollision(const GameObject2D& other) const
{
    auto otherPosition = other.GetPosition();
    float overlapX = std::min(
        otherPosition.x + other.GetRect().Width - m_Position.x,
        m_Position.x + m_Rect.WidthPixels - otherPosition.x
    );
    float overlapY = std::min(
        otherPosition.y + other.GetRect().Height - m_Position.y,
        m_Position.y + m_Rect.HeightPixels - otherPosition.y
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
