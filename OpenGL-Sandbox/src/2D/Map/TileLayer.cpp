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
    ComputeTileTexCoords();
}

void TileLayer::ComputeTileTexCoords()
{
    for (auto& ts : m_Tilesets)
    {
        float invTextureWidth = 1.0f / ts->Texture->GetWidth();
        float invTextureHeight = 1.0f / ts->Texture->GetHeight();

        for (int tileId = ts->FirstId; tileId <= ts->LastId; tileId++)
        {
            float tileRow = (ts->RowCount - 1) - (tileId / ts->ColumnCount);
            float tileColumn = (tileId % ts->ColumnCount) - 1;
            if (tileId % ts->ColumnCount == 0)
                tileRow++;

            float minU = tileColumn * ts->TileWidth * invTextureWidth;
            float minV = tileRow * ts->TileHeight * invTextureHeight;
            float maxU = (tileColumn + 1) * ts->TileWidth * invTextureWidth;
            float maxV = (tileRow + 1) * ts->TileHeight * invTextureHeight;

            // TODO : way to set the whole array?
            if (m_TileData.find(tileId) == m_TileData.end())
            {
                m_TileData[tileId] = CreateScope<Tile>();
            }
            m_TileData[tileId]->TexCoords[0] = glm::vec2(minU, minV);
            m_TileData[tileId]->TexCoords[1] = glm::vec2(maxU, minV);
            m_TileData[tileId]->TexCoords[2] = glm::vec2(maxU, maxV);
            m_TileData[tileId]->TexCoords[3] = glm::vec2(minU, maxV);

            // TODO : map with multiple tilesets
            /* if (m_Tilesets.size() > 1)
            {
                for (int i = 1; i < m_Tilesets.size(); i++)
                {
                    auto tileset = m_Tilesets[i];
                    if (tileId >= tileset->FirstId && tileId <= tileset->LastId)
                    {
                        tileId = tileId + tileset->TileCount - tileset->LastId;
                        tilesetIndex = i;
                        ts = tileset;
                        break;
                    }
                }
            } */

            m_TileData[tileId]->Obj.SetWidth(ts->TileWidth * invTextureWidth); // TODO : are these correct?
            m_TileData[tileId]->Obj.SetHeight(ts->TileHeight * invTextureHeight);
        }
    }
}

void TileLayer::OnUpdate(GLCore::Timestep timestep)
{
}

void TileLayer::OnRender()
{
    for (unsigned int y = 0; y < m_ColumnCount; y++)
    {
        for (unsigned int x = 0; x < m_RowCount; x++)
        {
            int tileId = m_TileMap[x][y];
            if (tileId == 0)
                continue;

            int tilesetIndex = 0;
            auto ts = m_Tilesets[0]; // TODO : support for multiple tilesets

            glm::vec3 position = glm::vec3(y, (m_ColumnCount - 1) - x, -0.05f); // TODO : magic number for z
            m_TileData[tileId]->Obj.SetPosition(position);
            Renderer2D::DrawQuad(position, { 1.0f, 1.0f }, ts->Texture, m_TileData[tileId]->TexCoords);
        }
    }
}

bool TileLayer::SolveCollision(GameObject2D& obj)
{
    for (unsigned int y = 0; y < m_ColumnCount; y++)
    {
        for (unsigned int x = 0; x < m_RowCount; x++)
        {
            int tileId = m_TileMap[x][y];
            if (tileId == 0)
                continue;

            int tilesetIndex = 0;
            auto ts = m_Tilesets[0]; // TODO : support for multiple tilesets

            if (m_TileData[tileId]->Obj.Intersects(obj))
            {
                auto objPosition = obj.GetPosition();
                auto resolution = m_TileData[tileId]->Obj.ResolveCollision(obj);
                obj.SetPosition(objPosition.x + resolution.x, objPosition.y + resolution.y);
                LOG_INFO("Player repositioned to: ({0}, {1})", objPosition.x + resolution.x, objPosition.y + resolution.y);
                return true;
            }
        }
    }
    return false;
}
