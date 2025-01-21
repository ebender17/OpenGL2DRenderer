#pragma once

#include <glm/glm.hpp>
#include <GLCore.h>

struct Rect
{
    glm::vec2 SpriteSize;
    float WidthPixels, HeightPixels;
    float Width, Height;

    Rect(const glm::vec2 spriteSize, float widthPixels, float heightPixels)
        : SpriteSize(spriteSize), WidthPixels(widthPixels), HeightPixels(heightPixels)
    {
    }
};

class GameObject2D
{
public:
    GameObject2D(const glm::vec3& position, const glm::vec2& spriteSize, float width, float height);

    virtual void OnEvent(GLCore::Event& event);

    bool Intersects(const GameObject2D& other) const;

    glm::vec2 ResolveCollision(const GameObject2D& other) const;

    void SetPosition(float x, float y) { m_Position = { x, y, m_Position.z }; }
    void SetPosition(const glm::vec3& position) { m_Position = position; }
    void SetRect(const Rect& rect) { m_Rect = rect; }

    const glm::vec3& GetPosition() const { return m_Position; }
    const glm::vec2& GetSpriteSize() const { return m_Rect.SpriteSize; }
    const Rect& GetRect() const { return m_Rect; }
private:
    bool OnWindowResized(GLCore::WindowResizeEvent& event);
protected:
    glm::vec3 m_Position;
    Rect m_Rect;
};