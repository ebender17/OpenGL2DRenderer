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

            int tilesetIndex = 1;
            // find tileset and tileId if using more than one tileset
            if (m_Tilesets.size() > 1) // TODO : test loading map with more than one tileset
            {
                for (int i = 1; i < m_Tilesets.size(); i++)
                {
                    Tileset* tileset = m_Tilesets[tilesetIndex];
                    if (tileId >= tileset->FirstId && tileId <= tileset->LastId)
                    {
                        tileId = tileId + tileset->TileCount - tileset->LastId;
                        tilesetIndex = i;
                        break;
                    }
                }
            }

            Tileset* ts = m_Tilesets[tilesetIndex];
            int tileRow = tileId / ts->ColumnCount;
            int tileColumn = tileId - tileRow * ts->ColumnCount - 1;

            // if this tile is on the last column
            if (tileId % ts->ColumnCount == 0)
            {
                tileRow--;
                tileColumn = ts->ColumnCount - 1;
            }

            glm::vec3 position = glm::vec3(x, y, -0.05);
            glm::vec2 size = glm::vec2(1.0f);
            glm::vec2 spriteSize = glm::vec2(1.0f);

            // 2. TODO : make sure tex coords are correct
            // TODO : cache texCoords for tiles using a map
            auto texture = ts->Texture;
            glm::vec2 minTexCoord = { (tileRow * ts->TileWidth) / texture->GetWidth(), (tileColumn * ts->TileHeight) / texture->GetHeight() };
            glm::vec2 maxTexCoord = { ((tileRow + spriteSize.x) * ts->TileWidth) / texture->GetWidth(), ((tileColumn + spriteSize.y) * ts->TileHeight) / texture->GetHeight() };
            glm::vec2 texCoords[4] = {
                { minTexCoord.x, minTexCoord.y },
                { maxTexCoord.x, minTexCoord.y },
                { maxTexCoord.x, maxTexCoord.y },
                { minTexCoord.x, maxTexCoord.y }
            }; // TODO
            Renderer2D::DrawQuad(position, size, ts->Texture, texCoords);

            // TODO : render error tile?
            // Renderer2D::DrawQuad({ x, y, -0.05f }, { 1.0f, 1.0f }, m_TextureErrorColor);
        }
    }
}
