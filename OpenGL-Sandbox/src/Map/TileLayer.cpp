#include "TileLayer.h"

#include <cmath>

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
            float tileColumn = ((tileId - 1) % ts->ColumnCount);
            if (tileId % ts->ColumnCount == 0)
                tileRow++;

            float minU = tileColumn * ts->TileWidth * invTextureWidth;
            float minV = tileRow * ts->TileHeight * invTextureHeight;
            float maxU = (tileColumn + 1) * ts->TileWidth * invTextureWidth;
            float maxV = (tileRow + 1) * ts->TileHeight * invTextureHeight;

            m_TileData[tileId].TexCoords[0] = glm::vec2(minU, minV);
            m_TileData[tileId].TexCoords[1] = glm::vec2(maxU, minV);
            m_TileData[tileId].TexCoords[2] = glm::vec2(maxU, maxV);
            m_TileData[tileId].TexCoords[3] = glm::vec2(minU, maxV);
        }
    }
}

void TileLayer::OnUpdate(GLCore::Timestep timestep)
{
}

void TileLayer::OnRender()
{
    // TODO : only issue draw calls for tiles inside camera bounds
    for (unsigned int y = 0; y < m_ColumnCount; y++)
    {
        for (unsigned int x = 0; x < m_RowCount; x++)
        {
            int tileId = m_TileMap[x][y];
            if (tileId == 0)
                continue;

            // TODO : support for multiple tilesets
            int tilesetIndex = 0;
            auto ts = m_Tilesets[0];

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

            glm::vec3 position = glm::vec3(y, x, -0.05f); // TODO : magic number for z
            Renderer2D::DrawQuad(position, c_SpriteSize, ts->Texture, m_TileData[tileId].TexCoords);
        }
    }
}

bool TileLayer::CheckCollision(const glm::vec2& objPosition, float width, float height)
{
    // TODO : support for multiple tilesets
    int tilesetIndex = 0;
    auto ts = m_Tilesets[0];

    // Convert object's bounding box to tile indices
    int startX = std::floor(objPosition.y) - 1;
    int startY = std::floor(objPosition.x);
    int endX = std::ceil(objPosition.y + (width / m_TileHeight));
    int endY = std::ceil(objPosition.x + (height / m_TileWidth));

    // Clamp to tile map bounds
    startX = std::max(0, startX);
    startY = std::max(0, startY);
    endX = std::min(static_cast<int>(m_RowCount - 1), endX);
    endY = std::min(static_cast<int>(m_ColumnCount - 1), endY);

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            int tileId = m_TileMap[x][y];
            if (tileId != 0) {
                if (IntersectsTile(glm::vec2(y, x), objPosition))
                {
                    LOG_INFO("Player intersection with tile at row: {0}, and col: {1} tileId: {2}", y, x, tileId);
                    return true;
                }
            }
        }
    }
    return false;
}

bool TileLayer::IntersectsTile(const glm::vec2& tilePosition, const glm::vec2& objPosition) const
{
    // collision x-axis
    bool collisionX = objPosition.x >= tilePosition.x;
    // collision y-axis
    bool collisionY = objPosition.y >= tilePosition.y;
    return collisionX && collisionY;
}