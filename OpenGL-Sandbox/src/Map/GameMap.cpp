#include "GameMap.h"

#include <glad/glad.h>

GameMap::GameMap(int rowCount, int columnCount, unsigned int reserveLayerCount)
    : m_RowCount(rowCount), m_ColumnCount(columnCount)
{
    m_Layers.reserve(reserveLayerCount);
}

GameMap::~GameMap()
{
    // TODO : need to delete textures?
}

void GameMap::OnUpdate(GLCore::Timestep timestep)
{
    for (unsigned int i = 0; i < m_Layers.size(); i++)
        m_Layers[i]->OnUpdate(timestep);
}

void GameMap::OnRender()
{
    for (unsigned int i = 0; i < m_Layers.size(); i++)
    {
        auto type = m_Layers[i]->GetTileLayerType();
        if (type == TileLayer::Type::Water) { continue; }
        m_Layers[i]->OnRender(c_GroundZPosition);
    }
}

void GameMap::OnRenderWaterLayer()
{
    for (unsigned int i = 0; i < m_Layers.size(); i++)
    {
        auto type = m_Layers[i]->GetTileLayerType();
        if (type != TileLayer::Type::Water) { continue; }
        m_Layers[i]->OnRender(c_WaterZPosition);
    }
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
