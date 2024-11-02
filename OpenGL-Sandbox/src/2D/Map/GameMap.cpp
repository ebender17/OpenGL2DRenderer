#include "GameMap.h"

GameMap::GameMap(unsigned int reserveLayerCount)
{
    m_MapLayers.reserve(reserveLayerCount);
}

void GameMap::OnUpdate(GLCore::Timestep timestep)
{
    for (unsigned int i = 0; i < m_MapLayers.size(); i++)
        m_MapLayers[i]->OnUpdate(timestep);
}

void GameMap::OnRender()
{
    for (unsigned int i = 0; i < m_MapLayers.size(); i++)
        m_MapLayers[i]->OnRender();
}
