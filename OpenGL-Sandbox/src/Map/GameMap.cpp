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

void GameMap::GenerateWaterMaskTexture()
{
    std::vector<int> tileData(m_RowCount * m_ColumnCount, 0);
    for (auto layer : m_Layers)
    {
        auto type = layer->GetTileLayerType();
        if (type != TileLayer::Type::Water) { continue; }
        int waterId = static_cast<int>(type);
        for (unsigned int y = 0; y < m_ColumnCount; ++y)
        {
            for (unsigned int x = 0; x < m_RowCount; ++x)
            {
                auto tilemap = layer->GetTileMap();
                int tileId = tilemap[x][y];
                if (tileId == 0)
                    continue;

                tileData[x * m_ColumnCount + y] = waterId;
                LOG_TRACE("isWater at {0}, row {1}, column {2}", y * m_RowCount + x, x, y);
            }
        }
    }

    glGenTextures(1, &m_WaterMaskTexture);
    glBindTexture(GL_TEXTURE_2D, m_WaterMaskTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_RowCount, m_ColumnCount,
        0, GL_RED_INTEGER, GL_INT, tileData.data());

    glBindTexture(GL_TEXTURE_2D, 0);
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
