#include "TileLayer.h"

TileLayer::TileLayer(int tileSize, int rowCount, int columnCount, const TileMap& tileMap, const TilesetList& tilesets)
    : m_TileSize(tileSize), m_RowCount(rowCount), m_ColumnCount(columnCount), m_TileMap(tileMap), m_Tilesets(tilesets)
{
    // TODO : use texture library to check if texture already exists, if not then load
}

void TileLayer::OnUpdate(GLCore::Timestep timestep)
{
}

void TileLayer::OnRender()
{
    for (unsigned int i = 0; i < m_RowCount; i++)
    {
        for (unsigned int j = 0; j < m_ColumnCount; j++)
        {
            int tileId = m_TileMap[i][j];

            if (tileId == 0)
                continue;
            
            int tilesetIndex = 0; // TODO : tileset indices start at 1 I believe. will this mess things up?
            if (m_Tilesets.size() > 1)
            {
                for (; tilesetIndex < m_Tilesets.size(); tilesetIndex++)
                {
                    Tileset* tileset = m_Tilesets[tilesetIndex];
                    if (tileId >= tileset->FirstId && tileId <= tileset->LastId)
                    {
                        tileId = tileId + tileset->TileCount - tileset->LastId; // TODO : double check this math with two tilesets
                        break;
                    }
                }
            }

            Tileset* ts = m_Tilesets[tilesetIndex];
            int tileRow = tileId / ts->ColumnCount;
            int tileColumn = tileId - tileRow * ts->ColumnCount; // TODO : do we need to subtract 1 here?

            // TODO : do we need this check?
            // if this tile is on the last column
            if (tileId % ts->ColumnCount == 0)
            {
                tileRow--;
                tileColumn = ts->ColumnCount - 1;
            }

            // TODO : LEFT OFF HERE RENDERING TILES
        }
    }
}
