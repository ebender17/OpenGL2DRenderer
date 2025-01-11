#include "TileLayer.h"

using namespace GLCore;

TileLayer::TileLayer(int tileWidth, int tileHeight, int rowCount, int columnCount, const TileMap& tileMap, const TilesetList& tilesets)
    : m_TileWidth(tileWidth), m_TileHeight(tileHeight), m_RowCount(rowCount), m_ColumnCount(columnCount), m_TileMap(tileMap), m_Tilesets(tilesets)
{
    // TODO : create a texture library (like the shader library) & check that first before loading
    for (unsigned int i = 0; i < m_Tilesets.size(); i++)
    {
        m_Tilesets[i]->Texture = Texture2D::Create(m_Tilesets[i]->Source);
    }
}

void TileLayer::OnUpdate(GLCore::Timestep timestep)
{
}

void TileLayer::OnRender()
{
    for (unsigned int x = 0; x < m_RowCount; x++)
    {
        for (unsigned int y = 0; y < m_ColumnCount; y++)
        {
            int tileId = m_TileMap[x][y];

            if (tileId == 0)
                continue;

            int tilesetIndex = 0;
            // find tileset and tileId if using more than one tileset
            if (m_Tilesets.size() > 1) // TODO : test loading map with more than one tileset
            {
                for (int i = 1; i < m_Tilesets.size(); i++)
                {
                    auto tileset = m_Tilesets[tilesetIndex];
                    if (tileId >= tileset->FirstId && tileId <= tileset->LastId)
                    {
                        tileId = tileId + tileset->TileCount - tileset->LastId;
                        tilesetIndex = i;
                        break;
                    }
                }
            }

            auto ts = m_Tilesets[tilesetIndex];
            float tileRow = ts->RowCount - 1 - (tileId / ts->ColumnCount);
            float tileColumn = (tileId % ts->ColumnCount) - 1;


            glm::vec2 spriteSize = glm::vec2(1.0f);
            // TODO : cache texCoords for tiles using a map
            float minU = static_cast<float>(tileColumn) / ts->ColumnCount;
            float minV = static_cast<float>(tileRow) / ts->RowCount;
            float maxU = static_cast<float>(tileColumn + spriteSize.x) / ts->ColumnCount;
            float maxV = static_cast<float>(tileRow + spriteSize.y) / ts->RowCount;
            glm::vec2 texCoords[4] = {
                { minU, minV },
                { maxU, minV },
                { maxU, maxV },
                { minU, maxV }
            };
            glm::vec3 position = glm::vec3(y, (m_ColumnCount - 1) - x, -0.05);
            auto texture = ts->Texture;
            Renderer2D::DrawQuad(position, { 1.0f, 1.0f }, ts->Texture, texCoords);
        }
    }
}
