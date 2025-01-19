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

void GameMap::SolveCollision(GameObject2D& obj) const
{
    for (unsigned int i = 0; i < m_Layers.size(); i++)
    {
        if (m_Layers[i]->GetCollision())
        {
            m_Layers[i]->SolveCollision(obj);
        }
    }
}