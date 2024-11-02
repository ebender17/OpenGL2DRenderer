#include "TileLayer.h"

TileLayer::TileLayer(int tileSize, int rowCount, int columnCount, const TileMap& tileMap, const TilesetList& tilesets)
    : m_TileSize(tileSize), m_RowCount(rowCount), m_ColumnCount(columnCount), m_TileMap(tileMap), m_TilesetList(tilesets)
{
}

void TileLayer::OnUpdate(GLCore::Timestep timestep)
{
}

void TileLayer::OnRender()
{
}
