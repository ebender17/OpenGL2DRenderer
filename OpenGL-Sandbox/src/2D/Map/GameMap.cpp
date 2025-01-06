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
