#include "GameMap.h"

GameMap::GameMap(unsigned int reserveLayerCount)
{
    m_Layers.reserve(reserveLayerCount);
}

void GameMap::OnUpdate(GLCore::Timestep timestep)
{
    for (unsigned int i = 0; i < m_Layers.size(); i++)
        m_Layers[i]->OnUpdate(timestep);
}

void GameMap::OnRender()
{
    for (unsigned int i = 0; i < m_Layers.size(); i++)
        m_Layers[i]->OnRender();
}

bool GameMap::CheckCollision(const glm::vec2& objPosition, float width, float height) const
{
    for (unsigned int i = 0; i < m_Layers.size(); i++)
    {
        if (m_Layers[i]->GetIsCollisionEnabled())
        {
            if (m_Layers[i]->CheckCollision(objPosition, width, height))
            {
                return true;
            }
        }
    }
    return false;
}
